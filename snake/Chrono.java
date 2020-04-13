public class Chrono {

    private long depart = 0;
    private long fin = 0;
    private long pause = 0;
    private long resume = 0;
    private long chrono = 0;

    public Chrono() {
    	depart = System.currentTimeMillis();
    	fin = 0;
    	pause = 0;
    	resume = 0;
    	chrono = 0;
    }

    public void chronoPause() {
    	pause = System.currentTimeMillis();
    }

    public void chronoResume() {
    	resume = System.currentTimeMillis();
    }
        
    public void chronoUpdate() {
        fin = System.currentTimeMillis();
        chrono = (fin - depart) - (resume - pause);
    }

    public long getChrono() {
        return chrono/1000;
    }       

    public String getChronoString() {
        return timeToHMS(getChrono());
    }

    public static String timeToHMS(long temp) {

        int h = (int) (temp / 3600);
        int m = (int) ((temp % 3600) / 60);
        int s = (int) (temp % 60);

        String time = "";

        if(h>0)
        	time += h + ":";
        if(m>9)
        	time += m + ":";
        else if(m>0)
        	time += "0" + m + ":";
        if(s>9)
        	time += s;
        else if(s>0)
        	time += "0" + s;

        return time;
        }

    }