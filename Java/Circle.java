import java.util.Scanner;

public class Circle {
    private double radius;

    public double getRadius() {
        return radius;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }

    //无参构造函数
    public Circle() {
        this.radius = 0.0;
    }

    //带参构造函数
    public Circle(double r) {
        this.radius = r;
    }

    public static void compare(double r1, double r2) {
        if (r1 < r2)
            System.out.println(r1 + "<" + r2);
        else if(r1>r2)
            System.out.println(r2 + "<" + r1);
        else{
            System.out.println(r2 + "=" + r1);
        }

    }

    public static void main(String[] args) {
        Circle circle1 = new Circle();
        circle1.radius = 5.0;
        Circle circle2 = new Circle(8.0);
        Circle circle3 = new Circle(10.0);
        Circle circle4 = new Circle(5.0);

        compare(circle1.radius,circle2.radius);
        compare(circle1.radius,circle3.radius);
        compare(circle1.radius,circle4.radius);
    }
}


