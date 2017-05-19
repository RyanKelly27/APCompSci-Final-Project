import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import com.fazecast.jSerialComm.*;
import javax.swing.*;

public class Preset_Tab extends Main_LED {
	
	public Preset_Tab(JPanel panel, ArrayList<Presets> presets)
	{
		setupPresetPanel(panel, presets);
	}

	public void setupPresetPanel(JPanel panel, ArrayList<Presets> presets)
	{
		presetModel = new DefaultListModel();
		for(int i=0; i<presets.size(); i++)
			presetModel.addElement(presets.get(i));
		presetList = new JList(presetModel);
		presetList.setLayoutOrientation(JList.VERTICAL);
		presetScroller = new JScrollPane(presetList);
		panel.add(presetScroller);
		
		JButton selectPreset = new JButton("Select Preset");
		selectPreset.setActionCommand("Select Preset");
		selectPreset.addActionListener(new ButtonClickListener());
		panel.add(selectPreset);
	}
	
	public void enablePreset(int preset)
	{
		try {
			output.write(1);
			output.write(preset);
		} catch (Exception e) {
			System.out.println("ERROR");
		}
	}
	
	private class ButtonClickListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e) 
		{
			String command = e.getActionCommand();
			
			if(command.equals("Select Preset"))
			{
				
			}
		}
	}
}
