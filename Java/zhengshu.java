import java.util.Scanner;

public class zhengshu
{
    public static void main(String args[])
    {
        int input = 0;
        while(input!=1)
        {
            Scanner scan = new Scanner(System.in);
            System.out.print("input number<1000:\n");
            input = scan.nextInt();

            if (input % 9 == 0 && input < 1000)
                System.out.print("can be devided.\n");
            else {
                System.out.print("cannot be devided.\n");
            }
        }
    }
}
