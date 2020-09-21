import os,re,shutil

#lines2 = [elt.replace("#include <QColor>","#include <QtGui/QColor>") for elt in lines]
#lines2 = [elt.replace("#include <QHideEvent>","#include <QtGui/QHideEvent>") for elt in lines]
#something = "QPainterPath"
#"QHideEvent","QPixmap","QRgb" QtWidgets

pat = re.compile("#include <(Q[A-Z][^\>]+)")

def isQtHeader(f):
    for r,dirs,fis in os.walk("/usr/include/x86_64-linux-gnu/qt5"):
        if f in fis:
            return os.path.join(os.path.basename(r),f)
    return None

def dealWith(fn):
    fn2 = "{}.new".format(fn)
    for something in ["QMainWindow"]:
        with open(fn) as f:
            lines = f.readlines()
            lines2 = []
            for line in lines:
                m = pat.match(line)
                if m:
                    p = m.group(1)
                    if "/" in p:
                        lines2.append(line)
                    else:
                        s = isQtHeader(p)
                        if s:
                            print(p,s)
                            lines2.append(line.replace(p,s))
                        else:
                            lines2.append(line)
                else:
                    lines2.append(line)
                pass
            pass
        #lines2 = [elt.replace("#include <{}>".format(something),"#include <QtWidgets/{}>".format(something)) for elt in lines]
        if lines2 != lines:
            with open(fn2,"w") as f:
                f.writelines(lines2)
            shutil.move(fn2,fn)

for r,dirs,fis in os.walk(os.getcwd()):
    for fi in fis:
        if os.path.splitext(fi)[1] in [".h",".cxx"]:
            dealWith(os.path.join(r,fi))



#QPainterPath
