import java.util.Scanner;

public class gcd
{
    public static int compare(int a,int b){
        if(a>b)
        {
            int temp=a;
            a=b;
            b=temp;
        }
        if(b%a==0)return a;
        else return compare(a,b%a);
    }

    public static void main(String args[]){
        int a,b;
        Scanner scan=new Scanner(System.in);
        System.out.print("input a：\n");
        a = scan.nextInt();
        System.out.print("input b：\n");
        b = scan.nextInt();
        System.out.println(compare(a,b));
    }
}
