package bataille;
import java.util.*;

public class Paquet {
	private Carte paquet[];
	//private List<Carte> paquet_;
	
	public Paquet() {
        paquet = new Carte[52];
        //paquet_ = new ArrayList<>();

        for (int i = 1; i < 5; i++) {
	        for (int j = 0; j < 13; j++) {
	        	paquet[13*(i-1) + j] = new Carte(i, j+1);
	        	//paquet_.add(new Carte(i, j));
	        }
        }
   }
	
	public void shuffle() {
        Random nb = new Random();
        //Collections.shuffle(paquet_, nb);

        for (int i = 0; i < 52; i++) {
             int rand = nb.nextInt(52);

             Carte temp = paquet[i];
             paquet[i] = paquet[rand];
             paquet[rand] = temp;
        }
   }
	
	
	public Carte[] getPaquet() {
		return paquet;
	}
	/*
	public List<Carte> getPaquet(int startIndex, int quantity)

	{
		List<Carte> paquetClone_ = new ArrayList<Carte>();
		int end = startIndex + quantity > paquetClone_.size() ? paquetClone_.size() : startIndex + quantity;
		
		for(int i = startIndex; i < end; i++)
			paquetClone_.add(paquet_.get(i));
		
		
		return paquetClone_;
	}
	*/

}

