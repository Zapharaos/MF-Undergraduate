package bataille;
import java.util.Random;

public class Main {
    private Carte hand[];
    private String name;
    private int size;

    // Mains des joueurs
    public Main(String name, Carte[] paquet, int player) {
    	int x = paquet.length;
    	this.size = x/2;
    	this.name = name;
    	this.hand = new Carte[size];
    	
        int start = player == 0 ? 0 : size;
        int end = player == 0 ? size : x;
        
        
        for (int i=start; i<end; i++) {
        	hand[i-start] = paquet[i];
        } 
    }
    
    // Main temporaire
    public Main() {
    	this.size = 1;
    	this.hand = new Carte[size];
    }
    
    public void HandShuffle(int length) {
        Random nb = new Random();

        for (int i = 0; i < length; i++) {
             int rand = nb.nextInt(length);

             Carte temp = hand[i];
             hand[i] = hand[rand];
             hand[rand] = temp;
        }
   }
    
    public void delCard(int index) {
    	editSize(-1);
    	Carte[] temp = new Carte[size];
    	for (int i=0; i<index; i++) {
    		temp[i] = hand[i];
    	}
    	for (int j=index; j<size; j++) {
    		temp[j] = hand[j+1];
    	}

    	hand = temp;
    }
    
    public void addCard(Carte last) {
    	editSize(+1);
    	Carte[] temp = new Carte[size];
    	for (int i=0; i<size-1; i++) {
    		temp[i] = hand[i];
    	}
    	temp[size-1] = last;
    	hand = temp;
    }
    
    public Carte[] getHand() {
    	return hand;
    }
    
    public int getIndexHand(int index) {
    	return hand[index].getValue();
    }
    
    public String getName() {
        return name;
    }
    
    public int getSize() {
    	//return hand.length;
    	return size;
    }
    
    public void editSize(int x) {
    	size = size + x;
    }
}