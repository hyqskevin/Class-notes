import java.util.Scanner;

public class narcissus
{
    public static boolean isNarcissus(int n)
    {
        int a,b,c,sum;//百,十,个
        a = n/100;//求百位
        b = (n/10)%10;//求十位
        c = (n%100)%10;//求个位
        sum = a*a*a+b*b*b+c*c*c;
        if (sum == n) {
            return true;
        }
        else {
            return false;
        }
    }

    public static void main(String args[]) {
        int input = 0;
        //test1:
        /*Scanner scan = new Scanner(System.in);
        System.out.print("input Narcissus:\n");
        input = scan.nextInt();
        if (isNarcissus(input))
            System.out.println(input+" is Narcissus.");
        else{
            System.out.println(input+" is not Narcissus.");
        }*/
        //test2:
        for (int i = 100; i < 1000; i++) {
            if (isNarcissus(i))
                System.out.println(i);
        }
    }
}
