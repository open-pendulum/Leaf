#!/usr/bin/env python3
"""
Dev Log Generator
生成符合 Leaf 项目格式的工作日志
"""

import os
import re
from datetime import datetime
from typing import List, Dict, Optional

class DevLogGenerator:
    def __init__(self, project_root: str):
        self.project_root = project_root
        self.dev_log_dir = os.path.join(project_root, "dev-log")

    def generate_log(self,
                    content: str,
                    changes: List[Dict] = None,
                    knowledge_points: List[Dict] = None,
                    improvements: List[str] = None,
                    debug_records: List[Dict] = None) -> str:
        """生成开发日志"""

        # 获取当前日期
        current_date = datetime.now().strftime("%Y-%m-%d")

        # 构建日志内容
        log_parts = [
            f"## 开发日志 - {current_date}",
            "",
            "### 本日变更概览"
        ]

        # 添加变更概览
        if changes:
            for change in changes:
                log_parts.append(f"- **{change['title']}**: {change['description']}")
                if 'details' in change and change['details']:
                    for detail in change['details']:
                        log_parts.append(f"  - {detail}")
        else:
            # 如果没有提供changes，从content中提取
            changes = self._extract_changes(content)
            if changes:
                for change in changes:
                    log_parts.append(f"- **{change['title']}**: {change['description']}")

        log_parts.extend([
            "",
            "### 核心知识点记录"
        ])

        # 添加知识点
        if knowledge_points:
            for point in knowledge_points:
                log_parts.append(f"#### {point['title']}")
                log_parts.append(point['content'])
        else:
            # 如果没有提供知识点，从content中提取
            knowledge_points = self._extract_knowledge_points(content)
            if knowledge_points:
                for point in knowledge_points:
                    log_parts.append(f"#### {point['title']}")
                    log_parts.append(point['content'])
            else:
                log_parts.append("（今日暂无特别的知识点记录）")

        log_parts.extend([
            "",
            "### 后续可以改进/扩展的点"
        ])

        # 添加改进点
        if improvements:
            for improvement in improvements:
                log_parts.append(f"- {improvement}")
        else:
            log_parts.append("- （待补充）")

        # 添加调试记录
        if debug_records:
            log_parts.extend([
                "",
                "### Debug 记录与问题解决"
            ])
            for i, record in enumerate(debug_records, 1):
                log_parts.extend([
                    f"#### 问题 {i}: {record['title']}",
                    f"**现象**: {record['description']}",
                    f"**原因分析**: {record['rootCause']}",
                    f"**解决方案**: {record['solution']}",
                    f"**经验教训**: {record['lessons']}"
                ])

        return "\n".join(log_parts)

    def _extract_changes(self, content: str) -> List[Dict]:
        """从内容中提取变更概览"""
        # 使用正则表达式提取可能的技术实现
        change_patterns = [
            (r'实现了?(\w+)[，。]', '实现功能'),
            (r'添加?了?(\w+)[，。]', '新增功能'),
            (r'修改?了?(\w+)[，。]', '功能修改'),
            (r'优化?了?(\w+)[，。]', '性能优化'),
            (r'修复?了?(\w+)问题', 'Bug修复'),
            (r'重构?了?(\w+)[，。]', '代码重构'),
        ]

        changes = []
        for pattern, title in change_patterns:
            matches = re.findall(pattern, content)
            if matches:
                changes.append({
                    'title': title,
                    'description': f"完成{matches[0]}相关的工作"
                })

        return changes if changes else [
            {
                'title': '日常开发',
                'description': '完成项目相关开发工作'
            }
        ]

    def _extract_knowledge_points(self, content: str) -> List[Dict]:
        """从内容中提取知识点"""
        knowledge_points = []

        # 查找技术相关的关键词
        tech_keywords = [
            'API', '接口', '架构', '设计模式', '算法',
            '渲染', '着色器', '矩阵', '变换', '相机',
            '缓冲区', '顶点', '索引', '纹理',
            '事件', '回调', '委托', '观察者模式',
            '内存管理', '性能优化', '调试'
        ]

        found_keywords = []
        for keyword in tech_keywords:
            if keyword in content:
                found_keywords.append(keyword)

        if found_keywords:
            knowledge_points.append({
                'title': '技术要点',
                'content': f"涉及技术栈：{', '.join(found_keywords[:3])}"  # 只取前3个
            })

        return knowledge_points

    def save_log(self, log_content: str, filename: Optional[str] = None):
        """保存日志到dev-log目录"""
        if not filename:
            current_date = datetime.now().strftime("%Y-%m-%d")
            filename = f"{current_date}.md"

        os.makedirs(self.dev_log_dir, exist_ok=True)
        filepath = os.path.join(self.dev_log_dir, filename)

        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(log_content)

        return filepath

def main():
    """主函数 - 当作为独立脚本运行时"""
    import argparse

    parser = argparse.ArgumentParser(description='生成开发日志')
    parser.add_argument('content', help='工作内容总结')
    parser.add_argument('--output', '-o', help='输出文件名')

    args = parser.parse_args()

    # 获取项目根目录（假设脚本在项目根目录的.claude/skills/下）
    project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

    generator = DevLogGenerator(project_root)

    # 生成日志
    log_content = generator.generate_log(args.content)

    # 保存日志
    filepath = generator.save_log(log_content, args.output)

    print(f"开发日志已生成: {filepath}")
    print("\n" + "="*50)
    print(log_content)
    print("="*50)

if __name__ == "__main__":
    main()