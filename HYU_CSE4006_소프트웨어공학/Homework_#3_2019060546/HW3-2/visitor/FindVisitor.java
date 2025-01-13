import java.io.*;
import java.util.List;
import java.util.ArrayList;

public class FindVisitor implements FileSystemVisitor {
    private String keyword;
    private List<String> foundItems;

    public FindVisitor(String keyword) {
        this.keyword = keyword.toLowerCase(); // 키워드를 소문자로 저장 (대소문자 구분 없게 처리)
        this.foundItems = new ArrayList<>();  // 검색 결과 저장 리스트 초기화
    }

    public void visitFileNode(FileNode fileNode) {
        // 파일 이름에 키워드가 포함되어 있는지 검사
        if (fileNode.getFile().getName().toLowerCase().contains(keyword)) {
            foundItems.add("File: " + fileNode.getFile().getAbsolutePath());
        }
    }

    public void visitDirectoryNode(DirectoryNode directoryNode) {
        // 디렉토리 이름에 키워드가 포함되어 있는지 검사
        if (directoryNode.getDirectory().getName().toLowerCase().contains(keyword)) {
            foundItems.add("Directory: " + directoryNode.getDirectory().getAbsolutePath());
        }
        // 하위 노드 재귀 방문
        for (FileSystemNode child : directoryNode.getChildren()) {
            child.accept(this);
        }
    }

    public List<String> getFoundItems() {
        return foundItems;
    }
}
