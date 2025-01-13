import java.io.*;

public class VisitorTester
{
   public static void main(String[] args)
   {
      DirectoryNode node = new DirectoryNode(new File(".."));
      node.accept(new PrintVisitor());

      String keyword = args[0];
      FindVisitor visitor = new FindVisitor(keyword); // 검색 키워드 전달
      node.accept(visitor);

      System.out.println();
      System.out.println("Find results:");
      for (String result : visitor.getFoundItems()) {
         System.out.println(result);
      }
   }
}
