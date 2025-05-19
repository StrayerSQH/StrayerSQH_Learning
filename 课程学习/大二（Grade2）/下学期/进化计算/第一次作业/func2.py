







#!!!!!!!!!!!!!!!!!!!!!!在代码最后可根据实际需要选择注释或取消注释以画出结果











import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# 环境
def environment(x1, x2):
    return 100 * (x1**2 - x2)**2 + (1 - x1)**2

# 表现形式转为二进制基因格式
def decimal_to_binary(decimal_value, low, high, genes):
    scaled_value = int((decimal_value - low) / (high - low) * (2**genes - 1))
    binary_string = format(scaled_value, f'0{genes}b')
    return binary_string

# 二进制基因格式转为表现形式
def binary_to_decimal(binary_string, low, high, genes):
    scaled_value = int(binary_string, 2)
    decimal_value = low + (scaled_value / (2**genes - 1)) * (high - low)
    return decimal_value

# 约束条件
LOW = -5.12
HIGHT = 5.12

# 初始化种群
population_size = 500  # 种群个体数
genes = 100            # 基因数
generation = 50        # 种群代数
mutation_prob = 0.2   # 变异概率
elite_prob = 0.02     # 精英概率
tournament_size = 5    # 锦标赛大小

population = np.random.uniform(LOW, HIGHT, (population_size, 2))

# 存储每代数据的元素
best_fitness_per_generation = []  # 每一代的最佳适应度
worst_fitness_per_generation = []  # 每一代的最差适应度
average_per_generation = []  # 每一代的平均适应度
std_dev=[]

# 创建画布
fig, ax = plt.subplots()
ax.set_xlim(LOW, HIGHT)
ax.set_ylim(LOW, HIGHT)
ax.set_xlabel('x1')
ax.set_ylabel('x2')
ax.set_title('Genetic Algorithm: Population over Generations')

# 绘制目标函数的热力图
x = np.linspace(LOW, HIGHT, 100)  # x 轴范围
y = np.linspace(LOW, HIGHT, 100)  # y 轴范围
X, Y = np.meshgrid(x, y)  # 生成网格点
Z = environment(X, Y)  # 计算目标函数值

# 使用 pcolormesh 绘制热力图
heatmap = ax.pcolormesh(X, Y, Z, cmap='viridis', shading='auto')
plt.colorbar(heatmap, label='f(x1, x2)')  # 添加颜色条

# 初始化散点图
scatter = ax.scatter([], [], color='red', s=10, alpha=0.5, label='Population')

def update(frame):
    global population

    fitness_degree = np.zeros((population_size, 1))

    # 计算适应度
    sum = 0
    for j in range(population_size):
        a = environment(population[j][0], population[j][1]) 
        sum += a
        #fitness_degree[j] = (52.4288 - a) / 52.4288 * 100 
        fitness_degree[j] = -a 
    
    # 计算标准差
    tmp=np.std(fitness_degree) # 计算适应度值的标准差
    std_dev.append(tmp)

    # 精英保留
    elite_size = int(population_size * elite_prob)
    elite_indices = np.argsort(fitness_degree.flatten())[-elite_size:]  
    elite_individuals = population[elite_indices]

    # 选择
    selected_indices = []
    for j in range(population_size - elite_size):
        candidates = np.random.choice(len(population), size=tournament_size, replace=False)
        # 选择适应度最高的个体
        best_candidate = candidates[np.argmax(fitness_degree[candidates])]
        selected_indices.append(best_candidate)
    selected_population = population[selected_indices]

    # 根据选择结果进行交配
    next_population = np.zeros((population_size - elite_size, 2))
    for j in range(0, population_size - elite_size - 1, 2): 
        # 选出父母
        parent1 = selected_population[j]
        parent2 = selected_population[j + 1]
        # 交叉融合基因
        crossover_point = []
        for k in range(4):
            crossover_point.append(np.random.randint(0, genes))
        child1_bin = [decimal_to_binary(parent1[0], LOW, HIGHT, genes)[:crossover_point[0]] + decimal_to_binary(parent2[0], LOW, HIGHT, genes)[crossover_point[0]:],
                      decimal_to_binary(parent1[1], LOW, HIGHT, genes)[:crossover_point[1]] + decimal_to_binary(parent2[1], LOW, HIGHT, genes)[crossover_point[1]:]]
        child2_bin = [decimal_to_binary(parent1[0], LOW, HIGHT, genes)[:crossover_point[2]] + decimal_to_binary(parent2[0], LOW, HIGHT, genes)[crossover_point[2]:],
                      decimal_to_binary(parent1[1], LOW, HIGHT, genes)[:crossover_point[3]] + decimal_to_binary(parent2[1], LOW, HIGHT, genes)[crossover_point[3]:]]
        child1 = [binary_to_decimal(gene, LOW, HIGHT, genes) for gene in child1_bin]
        child2 = [binary_to_decimal(gene, LOW, HIGHT, genes) for gene in child2_bin]
        next_population[j] = child1
        next_population[j + 1] = child2

    # 变异
    for j in range(len(next_population)):
        for k in range(2): 
            if np.random.rand() < mutation_prob:
                next_population[j][k] = np.random.uniform(LOW, HIGHT)

    # 更新种群并保存数据
    population = np.vstack((elite_individuals, next_population))
    best_fitness_per_generation.append(-(np.max(fitness_degree)))
    worst_fitness_per_generation.append(-(np.min(fitness_degree))) 
    average_per_generation.append(sum / population_size)

    
    # 更新散点图
    scatter.set_offsets(population)
    ax.set_title(f'Generation {frame+1}')
    return scatter,

ani = animation.FuncAnimation(fig, update, frames=generation, interval=20, blit=False, repeat=False)
plt.show()
plt.figure()
plt.plot(range(0, generation + 1), best_fitness_per_generation, label='Value', color='blue')
plt.plot(range(0, generation + 1), worst_fitness_per_generation, label='Worst', color='green')
plt.plot(range(0, generation + 1), average_per_generation, label='Average Value', color='red')
plt.plot(range(0, generation + 1), std_dev, label='Standard', color='red')
plt.xlabel('Generation')
plt.ylabel('Value')
plt.title('Fitness over Generations')
plt.legend()
plt.grid(True)
plt.show()