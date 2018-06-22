import java.util.Scanner;

public class paixu
{
  public void compare(int a, int b)
  {
    if (a>b)
      System.out.println(b+"<"+a);
    else
      System.out.println(a+"<"+b);
  }

  public void compare(double c, double d,double e)
  {
    if (c>d)
    {
      if (d>e)
        System.out.println(e+"<"+d+"<"+c);
      else
        System.out.println(d+"<"+e+"<"+c);
    }
    else
    {
      if (c>e)
        System.out.println(e+"<"+c+"<"+d);
      else
        System.out.println(c+"<"+d+"<"+e);
    }
  }

  public static void main(String[] args)
  {
    int a,b;
    double c,d,e;
    paixu p = new paixu();
    p.compare(2,4);
    p.compare(2.0,3.5,3.6);
  }
}


