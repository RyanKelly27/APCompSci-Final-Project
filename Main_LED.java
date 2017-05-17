import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 
import java.util.Enumeration;

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
	private OutputStream output;
	
	private ArrayList<String> listOfLEDs;
	
	public Main_LED()
	{
		listOfLEDs = new ArrayList<String>();
		for(int i=0; i<60; i++)
			listOfLEDs.add("LED "+(i+1));
		
		prepareGUI();
	}
	
	public void prepareGUI()
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
				
				
			}
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