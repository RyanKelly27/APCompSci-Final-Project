import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

public class Presets{
	
	public String name;
	public boolean color;
	public boolean delay;

	public Presets(String n, boolean c, boolean d)
	{
		name = n;
		color = c;
		delay = d;
	}
	
	public String getName()
	{
		return this.name;
	}
	
	public boolean requiresColor()
	{
		return this.color;
	}
	
	public boolean requiresDelay()
	{
		return this.delay;
	}
}
