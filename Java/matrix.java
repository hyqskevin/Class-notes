public class matrix
{
    public static void main(String args[])
    {
        int []matrix = {2,4,6,8,1,5,9,10,14,24,3};
        int min = matrix[0],max = matrix[0],sit = 0,sum = 0;
        for (int i=1; i<matrix.length-1; i++)
        {
            if (matrix[i]>max) {
                max = matrix[i];
                sit = i+1;
            }
        }
        System.out.println("min:"+max+" "+"in situation:"+sit);
        sit = 0;

        for(int j=1; j<matrix.length-1; j++) {
            if (matrix[j]<min){
                min = matrix[j];
                sit = j+1;
            }
        }
        System.out.println("min:"+min+" "+"in situation:"+sit);

        for(int k=1; k<matrix.length-1; k++){
            sum += matrix[k];
        }
        System.out.println("sum:"+sum);
    }
}
