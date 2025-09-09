import tkinter as tk
import os
from tkinter import messagebox

st = []
current_path = os.getcwd()
# 将当前目录添加到 sys.path
os.sys.path.insert(0, current_path)
def textRefresh():
    text.delete("1.0", tk.END)
    if len(st)>0:
        text.insert(tk.END, st[-1])
    
def genParseTree():
    content = text.get("1.0", tk.END)
    st.append(content)
    with open("mpatestIn.txt", "w", encoding="utf-8") as f:
        f.write(content)
    os.system("./mpaTest")
    with open("mpatestOut.txt", "r", encoding="utf-8") as f:
        res = f.read()
        st.append(res)
        textRefresh()
def prettify():
    content = text.get("1.0", tk.END)
    st.append(content)
    with open("mfmttestIn.txt", "w", encoding="utf-8") as f:
        f.write(content)
    os.system("./mfmtTest")
    with open("mfmttestOut.txt", "r", encoding="utf-8") as f:
        res = f.read()
        st.append(res)
        textRefresh()
def back():
    if len(st)>0:
        del st[-1]
        textRefresh()
        
    

# 创建窗口
root = tk.Tk()
root.title("源程序处理")
root.geometry("1200x800")

# 文本框
text = tk.Text(root, wrap=tk.WORD)
text.pack(expand=True, fill="both", padx=10, pady=10)

# 保存按钮
tk.Button(root, text="生成语法树", command=genParseTree).pack(pady=5)
tk.Button(root, text="美化代码", command=prettify).pack(pady=5)
tk.Button(root, text="撤回", command=back).pack(pady=5)

root.mainloop()
