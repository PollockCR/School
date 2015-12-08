// Imports
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.*; 
import java.lang.reflect.Field;
import javax.swing.event.*;

// Main ColorSampler class
public class ColorSampler extends JFrame
{
   // Class data members
   private static final long serialVersionUID = 1L;
   protected JList<String> listColors;
   protected DrawingTester drawTest;
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
   protected JLabel redLabel;
   protected JLabel greenLabel;
   protected JLabel blueLabel;
   protected CustomColor colorVals [];
   protected String colorNames [];
   protected CustomColor currentColor;
   protected int numColors;
   protected FileIO fileData;

   // CustomColor class
   class CustomColor extends Color
   {
      private static final long serialVersionUID = 1L;
      String name;
      
      public CustomColor()
      {
         super(0, 0, 0);
         name = " ";
      }
   }
   
   // Main method
   public static void main(String[] argv) throws IOException
   {
      new ColorSampler("Color Sampler");     
   }

   // Class Constructor
   public ColorSampler(String title) throws IOException
   {
      // Call constructor of base class
      super(title);   
      
      // Set window properties
      setBounds(100,100,600,400);
      getContentPane().setLayout(null);
      addWindowListener(new WindowDestroyer());
      
      // Allocate memory to objects and assign values
      buttonSave = new JButton("Save");
      buttonReset = new JButton("Reset");
      buttonRedMinus = new JButton("-");
      buttonRedPlus = new JButton("+");
      buttonGreenMinus = new JButton("-");
      buttonGreenPlus = new JButton("+");
      buttonBlueMinus = new JButton("-");
      buttonBluePlus = new JButton("+");
      tfRed = new JTextField("");
      tfGreen = new JTextField("");
      tfBlue = new JTextField("");
      redLabel = new JLabel("Red: ");
      greenLabel = new JLabel("Green: ");
      blueLabel = new JLabel("Blue: ");
      listColors = new JList<String>();
      drawTest = new DrawingTester();
      
      // Create action handler and listeners for objects
      buttonSave.addActionListener(new ActionHandler());  
      buttonReset.addActionListener(new ActionHandler());
      buttonRedMinus.addActionListener(new ActionHandler());  
      buttonRedPlus.addActionListener(new ActionHandler());
      buttonGreenMinus.addActionListener(new ActionHandler());  
      buttonGreenPlus.addActionListener(new ActionHandler());
      buttonBlueMinus.addActionListener(new ActionHandler());  
      buttonBluePlus.addActionListener(new ActionHandler());
      listColors.addListSelectionListener(new ListHandler());  
      
      // Add objects to program
      getContentPane().add(redLabel);
      getContentPane().add(tfRed);
      getContentPane().add(buttonRedMinus);
      getContentPane().add(buttonRedPlus);
      getContentPane().add(greenLabel);
      getContentPane().add(tfGreen);
      getContentPane().add(buttonGreenMinus);
      getContentPane().add(buttonGreenPlus);
      getContentPane().add(blueLabel);
      getContentPane().add(tfBlue);
      getContentPane().add(buttonBlueMinus);
      getContentPane().add(buttonBluePlus);
      getContentPane().add(buttonSave);
      getContentPane().add(buttonReset);
      getContentPane().add(listColors);
      getContentPane().add(drawTest);

      // Set boundaries for objects
      drawTest.setBounds(25, 25, 400, 165);
      buttonSave.setBounds(50, 325, 100, 25);
      buttonReset.setBounds(175, 325, 100, 25);
      blueLabel.setBounds(25, 275, 100, 25);
      greenLabel.setBounds(25, 245, 100, 25);
      redLabel.setBounds(25, 215, 100, 25);
      tfBlue.setBounds(130, 275, 100, 25);
      tfGreen.setBounds(130, 245, 100, 25);
      tfRed.setBounds(130, 215, 100, 25);
      buttonBlueMinus.setBounds(235, 275, 50, 25);
      buttonGreenMinus.setBounds(235, 245, 50, 25);
      buttonRedMinus.setBounds(235, 215, 50, 25);
      buttonBluePlus.setBounds(290, 275, 50, 25);
      buttonGreenPlus.setBounds(290, 245, 50, 25);
      buttonRedPlus.setBounds(290, 215, 50, 25);

      // Add window listener and destroyer
      addWindowListener(new WindowDestroyer());                 
      
      // Set default text field values
      tfRed.setText("255");
      tfGreen.setText("255");
      tfBlue.setText("0");
      
      // Allocate memory for color arrays
      colorVals = new CustomColor[20];
      colorNames = new String[20];
      
      // Set color values (file I/O not operational)
      //numColors = fileData.readData( colorVals, colorNames );
      String colorSet[] = {"Red", "Green", "Blue", "Yellow", "Cyan", "Magenta", "Orange", "Pink", "Gray", "Black", "White"};
      
      // Set list values with list boundaries
      listColors.setListData(colorSet);      
      listColors.setBounds(450, 25, 125, 325);
      setVisible(true);

   }


   // Define action listener                                       
   private class ActionHandler implements ActionListener 
   {      
      public void actionPerformed(ActionEvent e)
      {
         int colorVal;
         
         // save button - not yet operational
         if ( e.getSource() == buttonSave )
         {
            
         }
         
         // reset button
         else if ( e.getSource() == buttonReset )
         {
            // get current color
            int i = listColors.getSelectedIndex();
            String s = (String) listColors.getSelectedValue();
            
            // if a color is selected, display it
            if( i >= 0 )
            {
               Field field;
               Color curColor;
               try
               {
                  field = Class.forName("java.awt.Color").getField(s.toLowerCase());
                  curColor = (Color)field.get(null);
                  drawTest.setColor(curColor.getRed(), curColor.getGreen(), curColor.getBlue());
                  drawTest.repaint();
                  tfRed.setText(Integer.toString(curColor.getRed()));
                  tfGreen.setText(Integer.toString(curColor.getGreen()));
                  tfBlue.setText(Integer.toString(curColor.getBlue()));
               }
               catch (NoSuchFieldException | SecurityException | ClassNotFoundException | IllegalArgumentException | IllegalAccessException e1)
               {
              e1.printStackTrace();
            }
            }
            
            // if no color is selected, reset to yellow
            else
            {
                tfRed.setText("255");
                tfGreen.setText("255");
                tfBlue.setText("0");
                drawTest.setColor(255, 255, 0);
                drawTest.repaint();
            }
         }
         
         // red subtract button
         else if( e.getSource() == buttonRedMinus )
         {
            colorVal = drawTest.currentColor.getRed();
            if( colorVal > 0 )
            {
               colorVal -= 5;
               if( colorVal < 0 )
               {
                  colorVal = 0;
               }
               tfRed.setText( Integer.toString(colorVal) );
               drawTest.setColor(colorVal, drawTest.currentColor.getGreen(), drawTest.currentColor.getBlue());
               drawTest.repaint();
            }
         }
         
         // red addition button
         else if( e.getSource() == buttonRedPlus )
         {
            colorVal = drawTest.currentColor.getRed();
            if( colorVal < 255 )
            {
               colorVal += 5;
               if( colorVal > 255 )
               {
                  colorVal = 255;
               }
               tfRed.setText( Integer.toString(colorVal) );
               drawTest.setColor(colorVal, drawTest.currentColor.getGreen(), drawTest.currentColor.getBlue());
               drawTest.repaint();
            }
         }
         
         // green subtraction button
         else if( e.getSource() == buttonGreenMinus )
         {
            colorVal = drawTest.currentColor.getGreen();
            if( colorVal > 0 )
            {
               colorVal -= 5;
               if( colorVal < 0 )
               {
                  colorVal = 0;
               }
               tfGreen.setText( Integer.toString(colorVal) );
               drawTest.setColor(drawTest.currentColor.getRed(), colorVal, drawTest.currentColor.getBlue());
               drawTest.repaint();
            }
         }
         
         // green addition button
         else if( e.getSource() == buttonGreenPlus )
         {
            colorVal = drawTest.currentColor.getGreen();
            if( colorVal < 255 )
            {
               colorVal += 5;
               if( colorVal > 255 )
               {
                  colorVal = 255;
               }
               tfGreen.setText( Integer.toString(colorVal) );
               drawTest.setColor(drawTest.currentColor.getRed(), colorVal, drawTest.currentColor.getBlue());
               drawTest.repaint();
            }
         }
         
         // blue minus button
         else if( e.getSource() == buttonBlueMinus )
         {
            colorVal = drawTest.currentColor.getBlue();
            if( colorVal > 0 )
            {
               colorVal -= 5;
               if( colorVal < 0 )
               {
                  colorVal = 0;
               }
               tfBlue.setText( Integer.toString(colorVal) );
               drawTest.setColor(drawTest.currentColor.getRed(), drawTest.currentColor.getGreen(), colorVal);
               drawTest.repaint();
            }
         }
         
         // blue addition button
         else if( e.getSource() == buttonBluePlus )
         {
            colorVal = drawTest.currentColor.getBlue();
            if( colorVal < 255 )
            {
               colorVal += 5;
               if( colorVal > 255 )
               {
                  colorVal = 255;
               }
               tfBlue.setText( Integer.toString(colorVal) );
               drawTest.setColor(drawTest.currentColor.getRed(), drawTest.currentColor.getGreen(), colorVal);
               drawTest.repaint();
            }
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
     // check for new selection 
      public void valueChanged(ListSelectionEvent e)
      {
        Color curColor;
        if ( e.getSource() == listColors )
        {
           if ( !e.getValueIsAdjusting() )
           {
             // get selected value
             String s = (String) listColors.getSelectedValue();
              try
              {
                // display selection's color
                 Field field = Class.forName("java.awt.Color").getField(s.toLowerCase());
                 curColor = (Color)field.get(null);
                 drawTest.setColor(curColor.getRed(), curColor.getGreen(), curColor.getBlue());
                 drawTest.repaint();
                 tfRed.setText(Integer.toString(curColor.getRed()));
                 tfGreen.setText(Integer.toString(curColor.getGreen()));
                 tfBlue.setText(Integer.toString(curColor.getBlue()));
              }
              catch (Exception exc)
              {
                 System.out.println(exc);  
              }
           }
        }
      }
   }
   
   // DrawingTester class for displaying color
   private class DrawingTester extends JComponent
   {
      // Set default color value
      private static final long serialVersionUID = 1L;
      Color currentColor = new Color(255,255,0);
      
      // setColor function to set currentColor values
      public void setColor(int r, int g, int b)
      {
         currentColor = new Color(r,g,b);
      }
      
      // paint function to display color box
      public void paint(Graphics g)
      {
         Dimension d = getSize();

         g.setColor(currentColor);
         g.fillRect(1, 1, d.width-2, d.height-2);
         
         g.setColor(Color.black);
         g.drawRect(1, 1, d.width-2, d.height-2);
      }
   }

   // FileIO class for file input/output operations
   class FileIO
   { 
      int colorIndex;
      
      // readData method to save data from file to color arrays
      public int readData(CustomColor[] colorVals, String[] colorNames) throws IOException
      { 
         colorIndex = 0;
         FileInputStream stream = new FileInputStream("input.txt");  
         InputStreamReader reader = new InputStreamReader(stream); 
         StreamTokenizer tokens = new StreamTokenizer(reader); 
         String   n;
         int   r,g,b;
         while (tokens.nextToken() != StreamTokenizer.TT_EOF) 
         {  
            n = (String) tokens.sval; 
            tokens.nextToken(); 
            r = (int) tokens.nval; 
            tokens.nextToken(); 
            g = (int) tokens.nval;
            tokens.nextToken(); 
            b = (int) tokens.nval;
            colorIndex += 1;
            System.out.println(n + " " + r + " " + g + " " + b);  
         } 
         stream.close();
         return colorIndex;
      } 
      /**protected int colorIndex;
      
      public int readData(Color[] colorVals, String[] colorNames)
      {
         Scanner sc;
         try( sc = new Scanner(new FileInputStream("input.txt")) )
         {
            colorIndex = 0;
      
            while (sc.hasNext()) 
            {  
               colorVals[colorIndex].name = sc.next(); 
               colorNames[colorIndex] = colorVals[colorIndex].name;
               colorVals[colorIndex].red = sc.nextInt(); 
               colorVals[colorIndex].green = sc.nextInt(); 
               colorVals[colorIndex].blue = sc.nextInt(); 
               colorIndex += 1;
            } 
            System.out.println("File read."); 
            sc.close();
         } catch(FileNotFoundException ex) {}
         return colorIndex;
      }
      **/
      public void writeData(CustomColor[] colorVals) throws IOException
      {
         int index = 0;
         FileOutputStream ostream = new FileOutputStream("output.txt");
         PrintWriter writer = new PrintWriter(ostream); 
         
         while (index < colorVals.length) 
         {  
            writer.println(colorVals[index].name + " " + colorVals[index].getRed() + " " + colorVals[index].getGreen() + " " + colorVals[index].getBlue());  
         } 
         writer.flush();
         ostream.close(); 
         System.out.println("File written."); 
      } 
   }
}