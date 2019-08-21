# 	基于LAB颜色空间的超像素聚类算法

## 开发平台

| 开发平台     | x64  | x86  |
| ------------ | ---- | ---- |
| windows7     |      |      |
| windows10    |      |      |
| ubuntu16.04  |      |      |
| ubuntu 18.04 |      |      |

------

## 开发工具

| 开发工具 | 版本 |
| -------- | ---- |
| opencv   |      |
| qt       |      |
| cuda     |      |
| cmake    |      |
| Git      |      |

## 开发工作

### 算法公式

1. Desired number of superpixels ==  K

2. compactness factor==M，M[1,40],  default M=10

   ​						

   ​	
   $$
   Size_{SuperPixel}=0.5+\frac{Width*Height}{K}
   $$
   ​	

$$
STEP=\sqrt{Size_{SuperPixel}}+0.5
$$

$$
INVWT =\frac{1.0}{(STEP/M)^{2}}
$$

$$
D_{color}=(L_{seed}-L_{0})^{2}+(A_{seed}-A_{0})^{2}+(B_{seed}-B_{0})^{2}
$$

$$
D_{coord}=(X_{seed}-X_{0})^{2}+(Y_{seed}-Y_{0})^{2}
$$

$$
D=D_{color}+D_{coord}*INVWT
$$

​			

### 开发内容

1. LAB超像素聚类算法
2. Sigmoid 判断天空立面地面
3. 二次分割地面

