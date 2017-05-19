import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.*;


public class LED_Changer extends Main_LED{

	public LED_Changer(JPanel panel)
	{
		setupLEDPanel(panel);
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
	
	public int getIndex(String led)
	{
		for(int i=0; i<listOfLEDs.size(); i++)
		{
			if(listOfLEDs.get(i).equals(led))
				return i;
		}
		return 0;
	}
	
	public boolean alreadyInList(String name) {
        return addedLEDModel.contains(name);
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
		}	
	}
}
