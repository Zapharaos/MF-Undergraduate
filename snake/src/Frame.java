import javax.swing.JFrame;

public class Frame {
	
	public static final int WIDTH = 600, HEIGHT = 622;
	
	public Frame() {
		
		JFrame frame = new JFrame();
		Window window = new Window();
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setTitle("Snake");
		frame.setFocusable(true);
		frame.setSize(WIDTH, HEIGHT);
		frame.setResizable(false);
		frame.setLocationRelativeTo(null);
		
		frame.add(window);
		frame.addKeyListener(window);
		
		frame.setVisible(true);
		
	}
	
	public static void main(String[] args) {
		new Frame();
	}
}
