import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import com.fazecast.jSerialComm.*;


import javax.swing.*;

public class Main_LED {
	
	private DefaultListModel addedLEDModel;
	private JFrame mainFrame;
	private JTabbedPane tab;
	private JPanel LEDPanel;
	private JPanel presetPanel;
	private JComboBox LEDCombo;
	private JButton add;
	private JButton remove;
	private JButton clear;
	private JButton change;
	private JList addedLEDs;
	private JScrollPane addedScroller;
	private SerialPort selectedPort = null;
	private OutputStream output;
	
	private ArrayList<String> listOfLEDs;
	
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
		addedScroller = new JScrollPane(addedLEDs);
		
		panel.add(addedScroller);
	}
	
	public void setupPresetPanel(JPanel panel)
	{
		JButton selectPreset = new JButton("Select Preset");
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
				changeLEDColor(c, LEDCombo.getSelectedIndex());
			}
		}	
	}
	
	public void changeLEDColor(Color c, int led)
	{
		try {
			output.write(c.getRed());
			output.write(c.getGreen());
			output.write(c.getBlue());
			output.write(led);
		} catch (Exception e) {
			System.out.println("ERROR");
		}
	}
	
	protected boolean alreadyInList(String name) {
        return addedLEDModel.contains(name);
    }
	
	public static void main(String[] args)
	{
		Main_LED gui = new Main_LED();
	}
}