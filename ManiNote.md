```bash
# 建议是每个实验创建一个测试分支，例如对于util来说
git checkout util         # 切换到util分支
git checkout -b util_test # 建立并切换到util的测试分支

# 当你在util_test分支中每测试通过一个作业，请提交（git commit）你的代码，并将所做的修改合并（git merge）到util中，然后提交（git push）到github
git add .
git commit -m "完成了第一个作业"
git checkout util
git merge util_test
git push github util:util
```