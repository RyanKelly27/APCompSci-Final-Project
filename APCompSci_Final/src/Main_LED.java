import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import com.fazecast.jSerialComm.*;
import javax.swing.*;

public class Main_LED {
	
	public DefaultListModel addedLEDModel;
	public DefaultListModel presetModel;
	public JFrame mainFrame;
	public JTabbedPane tab;
	public JPanel LEDPanel;
	public JPanel presetPanel;
	public JComboBox LEDCombo;
	public JButton add;
	public JButton remove;
	public JButton clear;
	public JButton change;
	public JList addedLEDs;
	public JScrollPane addedScroller;
	public JList presetList;
	public JScrollPane presetScroller;
	public SerialPort selectedPort = null;
	public OutputStream output;
	public int delay;
	
	public ArrayList<String> listOfLEDs;
	public static ArrayList<Presets> listOfPresets;
	
	public Main_LED()
	{
		listOfLEDs = new ArrayList<String>();
		for(int i=0; i<60; i++)
			listOfLEDs.add("LED "+(i+1));
		
		SerialPort[] ports = SerialPort.getCommPorts();
		establishPort(ports);
	}
	
	public void establishPort(SerialPort[] ports)
	{
		JFrame portFrame = new JFrame("Port Selection");
		portFrame.setLayout(new FlowLayout());
		JComboBox portCombo = new JComboBox();
		
		for(int i=0; i<ports.length; i++)
			portCombo.addItem(ports[i].getSystemPortName());
		
		JButton selectPort = new JButton("Select Port");
		selectPort.addActionListener(new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					selectedPort = ports[portCombo.getSelectedIndex()];
					portFrame.dispose();
					selectedPort.openPort();
					selectedPort.setComPortTimeouts(selectedPort.TIMEOUT_WRITE_BLOCKING, 0, 0);
					output = selectedPort.getOutputStream();
					prepareGUI(selectedPort);
				}
			});
		
		portFrame.add(portCombo);
		portFrame.add(selectPort);
		portFrame.setVisible(true);
		portFrame.pack();
	}
	
	public void prepareGUI(SerialPort port)
	{
		mainFrame = new JFrame("LED_Settings");
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		prepareTabs(mainFrame.getContentPane());
		
		mainFrame.pack();
		mainFrame.setVisible(true);
	}
	
	public void prepareTabs(Container pane)
	{
		tab = new JTabbedPane();
		
		LEDPanel = new JPanel();
		setupLEDPanel(LEDPanel);
		
		presetPanel = new JPanel();
		setupPresetPanel(presetPanel);
		
		tab.add("LED", LEDPanel);
		tab.addTab("Presets", presetPanel);
		
		pane.add(tab, BorderLayout.CENTER);
	}
	
	public void setupLEDPanel(JPanel panel)
	{
		LEDCombo = new JComboBox();
		for(int i=0; i<listOfLEDs.size(); i++)
			LEDCombo.addItem(listOfLEDs.get(i));
		panel.add(LEDCombo);
		
		add = new JButton("Add");
		add.setActionCommand("Add");
	    add.addActionListener(new ButtonClickListener());
		panel.add(add);
		
		remove = new JButton("Remove");
		remove.setActionCommand("Remove");
		remove.addActionListener(new ButtonClickListener());
		panel.add(remove);
		
		clear = new JButton("Clear");
		clear.setActionCommand("Clear");
		clear.addActionListener(new ButtonClickListener());
		panel.add(clear);
		
		change = new JButton("Change");
		change.setActionCommand("Change");
		change.addActionListener(new ButtonClickListener());
		panel.add(change);
		
		addedLEDModel = new DefaultListModel(); 
		addedLEDs = new JList(addedLEDModel);
		addedLEDs.setLayoutOrientation(JList.VERTICAL);
		addedLEDs.setFixedCellWidth(200);
		addedScroller = new JScrollPane(addedLEDs);
		
		panel.add(addedScroller);
	}
	
	public void setupPresetPanel(JPanel panel)
	{
		presetModel = new DefaultListModel();
		for(int i=0; i<listOfPresets.size(); i++)
			presetModel.addElement(listOfPresets.get(i).getName());
		presetList = new JList(presetModel);
		presetList.setLayoutOrientation(JList.VERTICAL);
		presetScroller = new JScrollPane(presetList);
		panel.add(presetScroller);
		
		JButton selectPreset = new JButton("Select Preset");
		selectPreset.setActionCommand("Select Preset");
		selectPreset.addActionListener(new ButtonClickListener());
		panel.add(selectPreset);
	}
	
	private class ButtonClickListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e) 
		{
			String command = e.getActionCommand();
			
			if(command.equals("Add"))
			{
				if(alreadyInList(LEDCombo.getSelectedItem().toString()) != true)
					addedLEDModel.addElement(LEDCombo.getSelectedItem());
			}
			if(command.equals("Remove"))
			{
				if(addedLEDModel.isEmpty() != true && addedLEDs.getSelectedValue() != null)
				{
					int index = addedLEDs.getSelectedIndex();
		            addedLEDModel.remove(index);
		            addedLEDs.setSelectedIndex(index);
	                addedLEDs.ensureIndexIsVisible(index);
				}
			}
			if(command.equals("Clear"))
			{
				addedLEDModel.clear();
			}
			if(command.equals("Change"))
			{
				Color c;
				c = JColorChooser.showDialog(((Component)e.getSource()).getParent(), "Demo", Color.blue);
				changeLEDColor(c);
			}
			if(command.equals("Select Preset"))
			{
				enablePreset(presetList.getSelectedIndex(), ((Component)e.getSource()).getParent());
			}
		}	
	}
	
	public void changeLEDColor(Color c)
	{
		ArrayList<String> addedLED = new ArrayList<String>();
		for(int i=0; i<addedLEDModel.size(); i++)
			addedLED.add(i, addedLEDModel.getElementAt(i).toString());
		
		for(int i=0; i<addedLED.size(); i++)
		{
			try {
				output.write(0);
				output.write(getIndex(addedLED.get(i)));
				output.write(c.getRed());
				output.write(c.getGreen());
				output.write(c.getBlue());
				output.write(0);
				output.write(0);
			} catch (Exception e) {
				System.out.println("ERROR");
			}
		}
	}
	
	//Enables the Specified Preset
	public void enablePreset(int preset, Container parent)
	{	
		System.out.println(preset);
		
		try {
			output.write(1);	//This is the mode
			output.write(preset);	//This is the preset number/index of the listOfPresets
			if(listOfPresets.get(preset).requiresDelay())	//Checks to see if it requires a delay
			{
				if(listOfPresets.get(preset).requiresColor())	//Checks to see if it needs a color
				{
					Color c;
					c = JColorChooser.showDialog(parent, "Demo", Color.blue);	//Gets the color and outputs
					output.write(c.getRed());
					output.write(c.getGreen());
					output.write(c.getBlue());
				}
				delay = 0;
				
				JFrame delayFrame = new JFrame("Select Delay");		//Creates a new window to choose a delay
				delayFrame.setLayout(new FlowLayout());
				
				JSpinner delaySpinner = new JSpinner();		//Creates a spinner to select the delay value
				delayFrame.add(delaySpinner);
				
				JButton setDelay = new JButton("Select Delay");		//Creates the button to submit the delay value
				setDelay.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent e)
					{
						delay = (int) delaySpinner.getValue();
						if(delay >= 0)
							try {
								output.write(delay);
							} catch (IOException e1) {
								System.out.print("ERROR");
							}
					}
				});
				delayFrame.add(setDelay);
				
				delayFrame.setVisible(true);
			}
			else if(listOfPresets.get(preset).requiresColor())
			{
				Color c;
				c = JColorChooser.showDialog(parent, "Demo", Color.blue);
				output.write(c.getRed());
				output.write(c.getGreen());
				output.write(c.getBlue());
			}
				
		} catch (Exception e) {
			System.out.println("ERROR");
		}
	}
	
	public int getIndex(String led)
	{
		for(int i=0; i<listOfLEDs.size(); i++)
		{
			if(listOfLEDs.get(i).equals(led))
				return i;
		}
		return 0;
	}
	
	protected boolean alreadyInList(String name) {
        return addedLEDModel.contains(name);
    }
	
	public static void main(String[] args)
	{
		listOfPresets = new ArrayList<Presets>();
		listOfPresets.add(new Presets("Random Colors", false, true));
		listOfPresets.add(new Presets("Entire Spectrum", false, true));
		listOfPresets.add(new Presets("Wave Spectrum", false, true));
		listOfPresets.add(new Presets("Blink", true, true));
		listOfPresets.add(new Presets("Entire Strip", true, false));
		
		Main_LED gui = new Main_LED();
	}
}