public class judgement
{
    public static void main(String args[])
    {
        char x;//谁是好人
        int count = 0;
        for(x='A';x<='D';x++){
            boolean a=(x!='A');
            if (a==true) count++;
            boolean b=(x=='C');
            if (b==true) count++;
            boolean c=(x=='D');
            if (c==true) count++;
            boolean d=(x!='D');
            if (d==true) count++;

            //count = (x!='A')+(x=='C')+(x=='D')+(x!='D');
            if(count == 3) {
                System.out.println(x+"做好事");
                break;
            }
        }
    }
}
