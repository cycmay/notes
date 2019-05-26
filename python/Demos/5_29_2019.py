#! /usr/bin/python

def remove_duplicates(ori):
    list_new= list(set(ori))    # python set是一个不含重复元素的集合数据结构 
                                # 利用工厂函数一句话生成列表
    return list_new


# 对1，2，3，4 组合成三位数
def range_go():
    for x in range(1,5):
        for y in range(1,5):
            for z in range(1,5):
                if (x!=y) and (y!=z) and (z!=x):
                    print("%d %d %d" % (x, y, z))

if __name__ == "__main__":
    data = [1,2,3,1,2,3]
    print(remove_duplicates(data))
    range_go()
