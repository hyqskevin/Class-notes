class People {
    protected double height,weight;
    public void speakHello()
    {
        System.out.println("Hello.");
    }
    public void averageHeight()
    {
        System.out.println("");
    }
    public void averageWeight()
    {

    }
}

class ChinaPeople extends People{
    public void chinaGongfu()
    {

    }
    public void speakHello()
    {

    }
    public void averageHeight()
    {

    }
    public void averageWeight()
    {

    }
}

class AmericanPeople extends People{
    public void americanBoxing()
    {

    }
    public void speakHello()
    {

    }
    public void averageHeight()
    {

    }
    public void averageWeight()
    {

    }
}
public class TestFourOne{
    public static void main(String args[]){
        ChinaPeople c = new ChinaPeople();
        AmericanPeople a = new AmericanPeople();
        /*ChinaPeople.speakHello();
        AmericanPeople.speakHello();
        ChinaPeople.averageHeight();
        AmericanPeople.averageHeight();
        ChinaPeople.averageWeight();
        AmericanPeople.averageWeight();
        ChinaPeople.chinaGongfu();
        AmericanPeople.americanBoxing();*/
    }
}