import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.*; 
import java.util.Scanner;

import javax.swing.event.*;

public class ColorSampler extends JFrame
{
	protected JList<String> listColors;
	protected JButton buttonSave;
	protected JButton buttonReset;
	protected JButton buttonRedMinus;
	protected JButton buttonRedPlus;
	protected JButton buttonGreenMinus;
	protected JButton buttonGreenPlus;
	protected JButton buttonBlueMinus;
	protected JButton buttonBluePlus;
	protected JTextField tfRed;
	protected JTextField tfGreen;
	protected JTextField tfBlue;
	protected String colorVals [];
	protected int redVals [];
	protected int greenVals  [];
	protected int blueVals [];
	protected int numColors;
	protected FileIO fileData;

   public static void main(String[] argv) throws IOException
   {
      new ColorSampler("Color Sampler");     
   }

	public ColorSampler(String title) throws IOException
	{
		super(title);		// call constructor of base class
		setSize(550, 400);
		
		buttonSave = new JButton("Save");
		buttonReset = new JButton("Reset");
		buttonRedMinus = new JButton("-");
		buttonRedPlus = new JButton("+");
		buttonGreenMinus = new JButton("-");
		buttonGreenPlus = new JButton("+");
		buttonBlueMinus = new JButton("-");
		buttonBluePlus = new JButton("+");
		buttonSave.addActionListener(new ActionHandler());  
		buttonReset.addActionListener(new ActionHandler());

		tfRed = new JTextField("");
		tfGreen = new JTextField("");
		tfBlue = new JTextField("");
		
		listColors = new JList();
		listColors.addListSelectionListener(new ListHandler());  
		
		getContentPane().setLayout(new GridLayout(3, 4));
		getContentPane().add(new JLabel("Red:"));
		getContentPane().add(tfRed);
		getContentPane().add(buttonRedMinus);
		getContentPane().add(buttonRedPlus);
		getContentPane().add(new JLabel("Green:"));
		getContentPane().add(tfGreen);
		getContentPane().add(buttonGreenMinus);
		getContentPane().add(buttonGreenPlus);
		getContentPane().add(new JLabel("Blue:"));
		getContentPane().add(tfBlue);
		getContentPane().add(buttonBlueMinus);
		getContentPane().add(buttonBluePlus);
		
		getContentPane().setLayout(new GridLayout(2, 1));
		getContentPane().add(buttonSave);
		getContentPane().add(buttonReset);
		
		getContentPane().setLayout(new GridLayout(1, 1));
		getContentPane().add(new JScrollPane(listColors));

		
		addWindowListener(new WindowDestroyer());                 
		setVisible(true);
		
		tfRed.setText("255");
		tfGreen.setText("255");
		tfBlue.setText("0");
		
		colorVals = new String[100];
		redVals = new int[100];
		greenVals = new int[100];
		blueVals = new int[100];
		
		numColors = fileData.readData( colorVals, redVals, greenVals, blueVals );
		
		//String colors[] = {"Red", "Green", "Blue", "Yellow", "Cyan", "Magenta", "Orange", "Pink", "Grey", "Black", "White"};
		listColors.setListData(colorVals);

	}

	// Define action listener                                       
	private class ActionHandler implements ActionListener 
	{      
		public void actionPerformed(ActionEvent e)
		{
			if ( e.getSource() == buttonSave )
			{
				
			}
			else if ( e.getSource() == buttonReset )
			{
				tfRed.setText("255");
				tfGreen.setText("255");
				tfBlue.setText("0");
			}
		}
	}                        
	
	// Define window adapter                                       
	private class WindowDestroyer extends WindowAdapter 
	{      
		// implement only the function that you want
		public void windowClosing(WindowEvent e) 
		{    
			System.exit(0);  
		}                                                             
	}
	
	// Define list listener                                       
	private class ListHandler implements ListSelectionListener 
	{      
		public void valueChanged(ListSelectionEvent e)
		{
		  if ( e.getSource() == listColors )
		  {
		     if ( !e.getValueIsAdjusting() )
		     {
				int i = listColors.getSelectedIndex();
				String s = (String) listColors.getSelectedValue();
			 }
		  }
		}
	}

} 

class FileIO
{ 
	protected static Scanner reader;
	protected static FileOutputStream ostream;
	protected static PrintWriter writer;
	protected static int currentColor;
	
	public static void main(String argv[]) throws IOException 
	{ 
	} 
	
	public int readData(String[] colorVals, int[] redVals, int[] greenVals, int[] blueVals) throws IOException
	{
		Scanner reader = new Scanner(new FileInputStream("input.txt"));
		currentColor = 0;

		while (reader.hasNext()) 
		{  
			colorVals[currentColor] = reader.next(); 
			redVals[currentColor] = reader.nextInt(); 
			greenVals[currentColor] = reader.nextInt();
			blueVals[currentColor] = reader.nextInt();
			currentColor += 1;
		} 
		reader.;
		System.out.println("File read."); 
		return currentColor;
	}
	
	public void writeData(String[] colorVals, int[] redVals, int[] greenVals, int[] blueVals) throws IOException
	{
		int index = 0;
		ostream = new FileOutputStream("output.txt");
		writer = new PrintWriter(ostream); 
		
		while (index < currentColor) 
		{  
			writer.println(colorVals[index] + " " + redVals[index] + " " + greenVals[index] + " " + blueVals[index] );  
		} 
		writer.flush();
		ostream.close(); 
		System.out.println("File written."); 
	}

	
} 
