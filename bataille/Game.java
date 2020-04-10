package bataille;

public class Game {

	public static void main (String[] args) {
		int nb_tests = 1000;
		
		double nb_tours = 0, nb_fin = 0, nb_j1 = 0, nb_j2 = 0;
		
		for ( int test = 0; test < nb_tests; test++) {
			
			Paquet paquet = new Paquet();
			
			for(int i=0; i<1000; i++) {
				paquet.shuffle();
			}
		
			Main joueur1 = new Main("Matthieu", paquet.getPaquet(), 0);
			Main joueur2 = new Main("Ordi", paquet.getPaquet(), 1);
			Main multiple = new Main();
			
			int J1 = 0, J2 = 0, tours = 1, equal = 0;
			
			while(true) {
				
				tours++;
				
				if (joueur1.getSize() == 0) {
					nb_j2++;
					break;
				} else if (joueur2.getSize() == 0) {
					nb_j1++;
					break;
				} else if (tours == 10000) {
					nb_fin++;
					break;
				}
				
				if (J1 >= joueur1.getSize()) {
					joueur1.HandShuffle(joueur1.getSize());
					J1 = 0;
				}
				if (J2 >= joueur2.getSize()) {
					joueur2.HandShuffle(joueur2.getSize());
					J2 = 0;
				}
				
				Carte main1[] = joueur1.getHand();
				Carte main2[] = joueur2.getHand();
				Carte equals[] = multiple.getHand();
				
				int value1 = joueur1.getIndexHand(J1);
				int value2 = joueur2.getIndexHand(J2);
						
				if (equal == 2) {
					equal = 1;
					multiple.addCard(main1[J1]);
					multiple.addCard(main2[J2]);
					
					joueur1.delCard(J1);
					joueur2.delCard(J2);
				}
				else if (value1 > value2)
				{	
					if (equal == 1) {
						equal = 0;
						equals = multiple.getHand();
						for( int i=1; i < multiple.getSize(); i++) {
							joueur1.addCard(equals[i]);
						}
					}
					joueur1.addCard(main2[J2]);
					J1++;
					joueur2.delCard(J2);
					
				}
				else if (value1 < value2)
				{
					if (equal == 1) {
						equal = 0;
						equals = multiple.getHand();
						for( int i=1; i < multiple.getSize(); i++) {
							joueur2.addCard(equals[i]);
						}
					}
					joueur2.addCard(main1[J1]);
					J2++;
					joueur1.delCard(J1);
				}
				else {
					if (equal == 0) {
						multiple = new Main();
					}
					equal = 2;
					multiple.addCard(main1[J1]);
					multiple.addCard(main2[J2]);
					
					joueur1.delCard(J1);
					joueur2.delCard(J2);
				}
			}
			nb_tours += tours;
		}
		
		System.out.println("Nombre de tests = " + nb_tests + "\n");
		
		System.out.println("Nombre de tours totaux = " + nb_tours);
		nb_tours = nb_tours / nb_tests;
		System.out.println("Nombre de tours moyens = " + nb_tours + "\n");
		
		System.out.println("Nombre de victoire de J1 = " + nb_j1);
		System.out.println("Nombre de victoire de J1 = " + nb_j2);
		System.out.println("Nombre de partie non terminée = " + nb_fin + "\n");
		
		nb_j1 = (nb_j1 / nb_tests) * 100;
		nb_j2 = (nb_j2 / nb_tests) * 100;
		nb_fin = (nb_fin / nb_tests) * 100;
		
		System.out.println("Pourcentage de victoire de J1 = " + nb_j1);
		System.out.println("Pourcentage de victoire de J1 = " + nb_j2);
		System.out.println("Pourcentage de partie non terminée = " + nb_fin);
		
	}	
}




