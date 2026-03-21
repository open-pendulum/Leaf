# Dev Log Skill

This skill helps generate daily development logs following the established format used in the Leaf engine project.

## Usage
- Command: `/log` or `/dev-log`
- Triggers when user asks to "总结今天的工作内容" or "生成工作日志"

## Log Format

Based on existing dev-log files (2026-03-15.md, 2026-03-17.md, 2026-03-18.md), the format includes:

### 1. Development Log Title
```
## 开发日志 - YYYY-MM-DD
```

### 2. Daily Changes Overview
List main changes with bullet points:
```
### 本日变更概览
- **Feature Name**: Brief description
  - Detail implementation points
```

### 3. Core Knowledge Points
Document technical insights and learnings:
```
### 核心知识点记录
#### 1. Topic Title
- Technical explanation
- Code examples if relevant
```

### 4. Future Improvements
List potential enhancements:
```
### 后续可以改进/扩展的点
- Feature 1 description
- Feature 2 description
```

### 5. Debug Records (Optional)
Include problem-solving if issues were encountered:
```
### Debug 记录与问题解决
#### Problem X: Title
**Description**: What was the issue?
**Root Cause**: What caused it?
**Solution**: How was it fixed?
**Lessons Learned**: What to remember for future
```

## Key Sections to Include
1. **Development Log Title** - Always include with current date
2. **Daily Changes Overview** - Major features/changes implemented
3. **Core Knowledge Points** - Technical details and learnings
4. **Future Improvements** - Next steps and enhancements
5. **Debug Records** - If any problems were solved

## Examples from Existing Logs
- Shader system implementation with uniform management
- Renderer support for transform matrices
- Vertex attribute layout configurations
- Camera control implementation
- Debugging OpenGL vertex attribute issues