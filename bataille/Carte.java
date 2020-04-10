package bataille;

public class Carte {
    private int color; 
    private int value;
    
    public Carte(int color, int value) {
        this.color = color;
        this.value = value;
    }

    public int getColor() {
         return color;
    }

    public void setColor(int color) {
         this.color = color;
    }
    
    public int getValue() {
        return value;
   }

    public void setValue(int value) {
         this.value = value;
    }

    public String getCard() {
         String name = "";

         switch (value) {
	         case 1:
	        	 name = "A";
	        	 break;
	         case 11:
	        	 name = "J";
	        	 break;
	         case 12:
	        	 name = "Q";
	        	 break;
	         case 13:
	        	 name = "K";
	        	 break;
	         default:
	        	 name = Integer.toString(value);
	        	 break;
         }
         
         switch (color) {
	         case 1:
	        	 name += (char)'\u2663';
	        	 break;
	         case 2:
	        	 name += (char)'\u2666';
	        	 break;
	         case 3:
	        	 name += (char)'\u2764';
	        	 break;
	         case 4:
	        	 name += (char)'\u2660';
	        	 break;
         }

         return name;
    }

    public String toString() {
         return getCard();
    }
}