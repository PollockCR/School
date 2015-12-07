import java.io.*; 

public class FileIO
{ 
	public static void main(String argv[]) throws IOException 
	{ 
		FileInputStream istream = new FileInputStream("input.txt");  
		InputStreamReader reader = new InputStreamReader(istream); 
		StreamTokenizer tokens = new StreamTokenizer(reader); 
		FileOutputStream ostream = new FileOutputStream("output.txt");
		PrintWriter writer = new PrintWriter(ostream);     
		

		int currentColor = 0;
		while (tokens.nextToken() != tokens.TT_EOF) 
		{  
			colorVals[currentColor] = (String) tokens.sval; 
			tokens.nextToken(); 
			redVals[currentColor] = (int) tokens.nval; 
			tokens.nextToken(); 
			greenVals[currentColor] = (int) tokens.nval;
			tokens.nextToken(); 
			blueVals[currentColor] = (int) tokens.nval;
			
			writer.println(colorVals[currentColor] + " " + redVals[currentColor] + " " + greenVals[currentColor] + " " + blueVals[currentColor]); 
			
			currentColor += 1;
		} 
		
		writer.flush();
		istream.close();
		ostream.close(); 
		System.out.println("File written.");  
	} 
} 
