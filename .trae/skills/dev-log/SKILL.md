---
name: "dev-log"
description: "Records daily development work logs. Invoke when user asks to write/update dev-log or at end of day to summarize changes."
---

# Development Log Recorder

This skill helps record daily development work into the dev-log directory.

## When to Use

**Invoke this skill when:**
- User asks to "记录工作日志" (record work log)
- User asks to "写 dev-log" or "写入 dev-log"
- User asks to "总结今天的改动" (summarize today's changes)
- End of day work summary request

## Log File Location

```
dev-log/
```

## Log File Naming Convention

Format: `YYYY-MM-DD.md`

Examples:
- `2026-03-22.md`
- `2026-04-05.md`

## Log Format Template

```markdown
# 工作记录 - YYYY年M月D日

## 📋 今日概述

<Brief summary of today's main work>

## 🎯 主要改动

### 1. <Category Name>

#### 1.1 <Sub-category>
- **Files**: <list of affected files>
- **Description**: <what was changed>

### 2. <Another Category>
...

## 🔧 技术细节

### Key Code Changes
```cpp
// code example
```

## 📊 统计数据

- 新增文件: N 个
- 修改文件: N 个
- 删除文件: N 个

## 🎉 今日成果

1. <Achievement 1>
2. <Achievement 2>

## 🔍 问题与思考

### 问题
- <Issue encountered>

### 解决方案
- <How it was solved>
```

## How to Record a Dev Log

### Step 1: Check Git Status
```bash
git status
git diff --stat
git log --oneline -10
```

### Step 2: Review Changes
- Read modified files to understand what changed
- Identify new files (untracked)
- Note file renames and deletions

### Step 3: Write the Log
1. Create new file `dev-log/YYYY-MM-DD.md`
2. Follow the template format
3. Focus on **new** changes only (not previously existing work)
4. Include actual code examples from the changes

### Step 4: Verify
- Double-check that all mentioned changes actually exist in git status
- Remove any items that were not actually changed today

### Step 5: Add Comments (if code was documented)
- Check if any files were newly commented
- Note this in the log under "技术细节" section
- Example: "为 Renderer2D.cpp 中的关键函数添加了中文注释"
- **IMPORTANT**: Only add comments, do NOT modify any existing code logic
- **Comment style**: Add comments on the line ABOVE the code, not on the same line
  - Good: `// Comment here\ncode;`
  - Bad: `code;  // Comment here`

## Important Notes

- **Only record NEW changes** - Do not include work done on previous days
- **Be specific** - Include file paths and actual code snippets
- **Chinese language** - Use Chinese for the log content (as per project convention)
- **Reference previous logs** - Follow the format of existing logs in `dev-log/` directory
- **Add comments to code** - When documenting code changes, add inline comments explaining the logic
- **Do NOT modify code** - Only add comments, do not change any existing code
- **Log comment activity** - If comments were added, note this in the dev-log under "技术细节"
