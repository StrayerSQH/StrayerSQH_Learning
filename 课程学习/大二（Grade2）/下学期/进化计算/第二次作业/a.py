import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import random
from deap import base, creator, tools, algorithms
from deap.benchmarks.tools import hypervolume
from scipy.spatial import distance
import math

# 问题定义 - 使用ZDT3测试问题
def zdt3(individual):
    f1 = individual[0]
    g = 1 + 9.0 * np.sum(individual[1:]) / (len(individual) - 1)
    h = 1 - math.sqrt(f1 / g) - (f1 / g) * math.sin(10 * math.pi * f1)
    f2 = g * h
    return f1, f2

# 创建适应度和个体类
creator.create("FitnessMin", base.Fitness, weights=(-1.0, -1.0))
creator.create("Individual", list, fitness=creator.FitnessMin)

# 初始化工具盒
toolbox = base.Toolbox()

# 定义变量范围为[0,1]
BOUND_LOW, BOUND_UP = 0.0, 1.0

# 注册属性、个体和种群生成函数
NDIM = 30  # 变量维度
toolbox.register("attr_float", random.uniform, BOUND_LOW, BOUND_UP)
toolbox.register("individual", tools.initRepeat, creator.Individual, toolbox.attr_float, n=NDIM)
toolbox.register("population", tools.initRepeat, list, toolbox.individual)

# 注册评估函数
toolbox.register("evaluate", zdt3)

# MOEA/D参数
N = 500  # 种群大小
MAX_GEN = 30  # 最大代数
T = 50  # 邻域大小
CROSS_PROB = 0.9  # 交叉概率
MUT_PROB = 0.99  # 变异概率
ETA = 5  # 变异分布指数

# 生成权重向量
def generate_weights(num_weights, num_objectives):
    weights = np.random.dirichlet(np.ones(num_objectives), num_weights)
    return weights

# 计算邻域
def compute_neighbors(weights, T):
    num_weights = len(weights)
    neighbors = np.argsort(np.sum((weights[:, None] - weights) ** 2, axis=2))[:, :T]
    return neighbors

# 计算性能指标
def calculate_metrics(population, true_pf):
    # 获取目标值
    fits = np.array([ind.fitness.values for ind in population])
    
    # Hypervolume计算
    ref_point = np.array([1.1, 1.1])  # 参考点需要大于所有解
    hv = hypervolume(population, ref_point)
    
    # IGD计算
    dist_matrix = distance.cdist(fits, true_pf, 'euclidean')
    min_dist = np.min(dist_matrix, axis=1)
    igd = np.mean(min_dist)
    
    # Spread计算
    d_f = np.linalg.norm(fits[0] - fits[-1])
    d_l = np.mean([np.linalg.norm(fits[i] - fits[i + 1]) for i in range(len(fits) - 1)])
    spread = (d_f + np.sum(np.abs([np.linalg.norm(fits[i] - fits[i + 1]) - d_l for i in range(len(fits) - 1)]))) / (d_f + (len(fits) - 1) * d_l)
    
    return hv, igd, spread

# 计算ZDT3的真实Pareto前沿
def true_pareto_front_zdt3(n_points=1000):
    f1 = np.linspace(0, 1, n_points)
    f2 = 1 - np.sqrt(f1) - f1 * np.sin(10 * np.pi * f1)
    return np.column_stack((f1, f2))

# 注册遗传算子
toolbox.register("mate", tools.cxSimulatedBinaryBounded, low=BOUND_LOW, up=BOUND_UP, eta=ETA)
toolbox.register("mutate", tools.mutPolynomialBounded, low=BOUND_LOW, up=BOUND_UP, eta=ETA, indpb=1.0 / NDIM)
toolbox.register("select", tools.selRandom)

# MOEA/D算法主函数
def moead(population, toolbox, max_gen, T, cross_prob, mut_prob, weights, neighbors):
    # 初始化
    fitnesses = toolbox.map(toolbox.evaluate, population)
    for ind, fit in zip(population, fitnesses):
        ind.fitness.values = fit

    # 存储外部存档
    archive = tools.ParetoFront()
    archive.update(population)

    # 每一代的性能指标
    metrics_history = {"hv": [], "igd": [], "spread": [], "gen": []}
    true_pf = true_pareto_front_zdt3()

    for gen in range(max_gen):
        for i in range(len(population)):
            # 选择邻域中的两个个体
            k, l = np.random.choice(neighbors[i], 2, replace=False)
            parent1, parent2 = population[k], population[l]

            # 交叉
            if random.random() < cross_prob:
                child = toolbox.clone(parent1)
                child = toolbox.mate(child, parent2)[0]
                del child.fitness.values
            else:
                child = toolbox.clone(parent1)

            # 变异
            if random.random() < mut_prob:
                child = toolbox.mutate(child)[0]
                del child.fitness.values

            # 评估新个体
            child.fitness.values = toolbox.evaluate(child)

            # 更新邻域解
            for j in neighbors[i]:
                if child.fitness.dominates(population[j].fitness):
                    population[j] = toolbox.clone(child)
                elif not population[j].fitness.dominates(child.fitness):
                    # 基于聚合函数比较
                    f_child = np.sum(np.array(child.fitness.values) * weights[j])
                    f_current = np.sum(np.array(population[j].fitness.values) * weights[j])
                    if f_child < f_current:
                        population[j] = toolbox.clone(child)

        # 更新存档
        archive.update(population)

        # 计算性能指标
        hv, igd, spread = calculate_metrics(archive, true_pf)
        metrics_history["hv"].append(hv)
        metrics_history["igd"].append(igd)
        metrics_history["spread"].append(spread)
        metrics_history["gen"].append(gen)

        print(f"Gen {gen + 1}/{max_gen}: HV={hv:.4f}, IGD={igd:.4f}, Spread={spread:.4f}")

    return population, archive, metrics_history

# 主程序
if __name__ == "__main__":
    random.seed(42)
    weights = generate_weights(N, 2)
    neighbors = compute_neighbors(weights, T)

    # 初始化种群
    population = toolbox.population(n=N)

    # 运行MOEA/D算法
    final_pop, final_archive, metrics_history = moead(population, toolbox, MAX_GEN, T, CROSS_PROB, MUT_PROB, weights, neighbors)

    # 输出最终结果
    print("\nFinal Results:")
    print(f"Hypervolume: {metrics_history['hv'][-1]:.4f}")
    print(f"IGD: {metrics_history['igd'][-1]:.4f}")
    print(f"Spread: {metrics_history['spread'][-1]:.4f}")

    # 绘制最终Pareto前沿
    plt.figure(figsize=(12, 6))
    plt.scatter([ind.fitness.values[0] for ind in final_archive], [ind.fitness.values[1] for ind in final_archive], c='blue', alpha=0.5, label='Final Pareto Front')
    true_pf = true_pareto_front_zdt3()
    plt.plot(true_pf[:, 0], true_pf[:, 1], 'r--', label='True Pareto Front')
    plt.xlabel('f1')
    plt.ylabel('f2')
    plt.title('Final Pareto Front Comparison (ZDT3)')
    plt.legend()
    plt.grid(True)
    plt.show()

    # 绘制性能指标的进化曲线
    plt.figure(figsize=(12, 6))
    plt.plot(metrics_history['gen'], metrics_history['hv'], 'b-', label='Hypervolume (HV)')
    plt.plot(metrics_history['gen'], metrics_history['igd'], 'r-', label='IGD')
    plt.plot(metrics_history['gen'], metrics_history['spread'], 'g-', label='Spread')
    plt.xlabel('Generation')
    plt.ylabel('Metric Value')
    plt.title('Performance Metrics Evolution')
    plt.legend()
    plt.grid(True)
    plt.show()