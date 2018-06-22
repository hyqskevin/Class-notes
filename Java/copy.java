import java.util.Arrays;
public class copy {
    public static void main(String args[]) {
        int[] matrixa = new int[10];
        int[] matrixb = new int[10];
        int[] matrixc = new int[10];
        System.out.println(matrixa);
        Arrays.fill(matrixa,0,3,1);
        Arrays.fill(matrixa,4,9,2);
        //System.out.println(matrixa);
        System.arraycopy(matrixa,0,matrixb,0,5);
        matrixc = matrixa.clone();
        System.out.println(matrixb);
        System.out.println(matrixc);
    }
}
