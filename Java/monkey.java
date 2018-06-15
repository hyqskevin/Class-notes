public class monkey
{
    public String name;
    public int weight = 0;
    public int age = 0;
    public static String hobby = "eat banana";

    monkey()
    {
        name = "a";
        weight = 20;
        age = 1;
    }

    monkey(String a, int b, int c)
    {
        name = a;
        weight = b;
        age = c;
    }

    monkey(monkey m)
    {
        name = m.name;
        weight = m.weight;
        age = m.age;
    }

    public static void climbTree(String name, int high)
    {
        System.out.println(name +' '+ high);
    }

    public static void main(String arg[])
    {
        monkey m1 = new monkey("甲",50,3);
        monkey m2 = new monkey();
        m2.name = "乙";
        m2.weight = 45;
        m2.age = 2;

        System.out.println(m1.name+" "+m1.weight+" "+m1.age+" "+hobby);
        System.out.println(m2.name+" "+m2.weight+" "+m2.age+" "+hobby);

        climbTree("甲",10);
        climbTree("乙",5);


    }
}
