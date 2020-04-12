import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.*;
import java.util.ArrayList;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Point;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.Timer;

public class Window extends JPanel implements KeyListener{

	private static final long serialVersionUID = 1L;
	private int cell_width = 20, cell_height = 20, cell = 30;
	
	private int run = 1;
	private boolean pause = false;
	private boolean restart = false;
	
	public JButton restart_button = new JButton("RESTART");
	public JButton resume_button = new JButton("RESUME");
	public JButton quit_button = new JButton("QUIT GAME");
	
	private int size;
	private int direction;
	private ArrayList<Point> snake = new ArrayList<Point>();
	private Point apple;
	
	private Timer loop = new Timer(100, new ActionListener() {
		@Override
		public void actionPerformed(ActionEvent e) {
			
			remove(restart_button);
			remove(resume_button);
			remove(quit_button);
			
			if(!pause)
				tick();
			if(restart) {
				start();
			}
			
			repaint();
		}
	});

	public Window() {
		start();
		loop.start();
	}
	
	private void start() {
		snake.clear();
		size = 1;
		snake.add(new Point(cell_width/2, cell_height/2));
		apple = randApple();
		direction = 0;
		run = 1;
		restart = false;
	}
	
	private Point randApple() {
		Point temp = null;
		
		looper:
		while(temp == null) {
			int x = (int)(Math.random()*cell_width);
			int y = (int)(Math.random()*cell_height);
			
			for(int i = 0; i < size; i++) {
				if(snake.get(i).x == x && snake.get(i).y == y)
					continue looper;
			}
			temp = new Point(x, y);
		}
		return temp;
	}
	
	private void tick() {
		
		for(int i = size -1; i > 0; i--) {
			snake.get(i).x = snake.get(i -1).x;
			snake.get(i).y = snake.get(i -1).y;
		}
		
		switch (direction) {
		
			case 0:
				snake.get(0).y --;
				break;
				
			case 1:
				snake.get(0).y ++;
				break;
				
			case 2:
				snake.get(0).x ++;
				break;
				
			case 3:
				snake.get(0).x --;
				break;
		}
		
		if(snake.get(0).x == apple.x && snake.get(0).y == apple.y) {
			snake.add(new Point(snake.get(size -1).x, snake.get(size - 1).y));
			size ++;
			apple = randApple();
		}
		
		if(snake.get(0).x > cell_width -1 || snake.get(0).x < 0 || snake.get(0).y > cell_height -1 || snake.get(0).y < 0)
			run = 2;
		
		for(int i = 2; i < size; i++) {
			if(snake.get(0).x == snake.get(i).x && snake.get(0).y == snake.get(i).y) {
				run = 2;
				break;
			}
		}
	}
	
	@Override
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
		
		Graphics2D g2d = (Graphics2D)g;
		g2d.setStroke(new BasicStroke(2));
		g2d.fillRect(0, 0, Frame.WIDTH, Frame.HEIGHT);
		
		if (pause) {
			
			Font font = new Font("Serif", Font.PLAIN, 50); 
			g2d.setFont(font);
			
			g2d.setColor(Color.WHITE);
			g2d.drawString("PAUSED",200,100);
			
			font = new Font("Serif", Font.PLAIN, 30); 
			g2d.setFont(font);
			
			resume_button.addActionListener(new ActionListener() {
			      public void actionPerformed(ActionEvent e) {
			    	  pause = false;
			      }
			    });
			
			quit_button.addActionListener(new ActionListener() {
			      public void actionPerformed(ActionEvent e) {
			    	  loop.stop();
			    	  System.exit(0);
			      }
			    });
			
			resume_button.setBounds(200,220,200, 40); 
			quit_button.setBounds(200,260,200, 40);
			
			add(resume_button);
			add(quit_button);  
			
		} else if (run == 1) {
			
			for(int i = 0; i < cell_width; i++)
				for(int j = 0; j < cell_height; j++)
					g.drawRect(i*cell, j*cell, cell, cell);
			
			g2d.setColor(Color.RED);
			g2d.fillRect(apple.x*cell, apple.y*cell, cell, cell);
			
			g2d.setColor(Color.GREEN);
			g2d.fillRect(snake.get(0).x*cell, snake.get(0).y*cell, cell, cell);
			
			g2d.setColor(Color.YELLOW);
			for(int i = 1; i < size; i++)
				g2d.fillRect(snake.get(i).x*cell, snake.get(i).y*cell, cell, cell);
			
		} else {
			
			Font font = new Font("Serif", Font.PLAIN, 50); 
			g2d.setFont(font);
			
			g2d.setColor(Color.WHITE);
			g2d.drawString("YOU DIED",180,100);
			
			font = new Font("Serif", Font.PLAIN, 30); 
			g2d.setFont(font);
			
			g2d.drawString("Size = " + size,250,130);
			
			restart_button.addActionListener(new ActionListener() {
			      public void actionPerformed(ActionEvent e) {
			    	  restart = true;
			    	  run = 1;
			      }
			    });
			
			quit_button.addActionListener(new ActionListener() {
			      public void actionPerformed(ActionEvent e) {
			    	  loop.stop();
			    	  System.exit(0);
			      }
			    });
			
			restart_button.setBounds(200,220,200, 40); 
			quit_button.setBounds(200,260,200, 40);
			
			add(restart_button);
			add(quit_button);   
		}
				
	}
	
	@Override
	public void keyPressed(KeyEvent e) {
		
		int key = e.getKeyCode();
		
		switch(key) {
		
			case KeyEvent.VK_UP:
			case KeyEvent.VK_Z:
				if (direction == 1)
					run = 2;
				direction = 0;
				break;
				
			case KeyEvent.VK_DOWN:
			case KeyEvent.VK_S:
				if (direction == 0)
					run = 2;
				direction = 1;
				break;
				
			case KeyEvent.VK_RIGHT:
			case KeyEvent.VK_D:
				if (direction == 3)
					run = 2;
				direction = 2;
				break;
				
			case KeyEvent.VK_LEFT:
			case KeyEvent.VK_Q:
				if (direction == 2)
					run = 2;
				direction = 3;
				break;
				
			case KeyEvent.VK_SPACE:
			case KeyEvent.VK_ESCAPE:
				if(pause == false) {
					pause = true;
				} else {
					pause = false;
				}
				break;
		}
	}
	
	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
}

