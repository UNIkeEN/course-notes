import random
import sys
from tqdm import tqdm

datasets = []

MATCH_COST = 0
MISMATCH_COST = 5
GAP_COST = 3

def load_dataset():
    with open("MSA_database.txt") as f:
        for line in f:
            datasets.append(line)

def GA(query, target1, target2, expand_len, population_size, generation, num_mutate):
    """
    @params
    query、target1, target2: 待匹配串
    expand_len: 扩展倍数
    population: 种群规模
    generation: 进化代数
    num_mutate: 变异次数（每次变异对一个个体的三条序列都进行操作）
    """

    len_a = len(query)
    len_b = len(target1)
    len_c = len(target2)
    len_i = int(max(len_a, len_b, len_c) * expand_len) # 对比序列长度
    num_blank_a = len_i - len_a
    num_blank_b = len_i - len_b
    num_blank_c = len_i - len_c

    population = [] # 种群

    def generate_seq(len, num_blank):    # 生成单个序列
        seq = [1] * len
        zeros = random.sample(range(len), num_blank)
        for pos in zeros:
            seq[pos] = 0

        return seq

    def calc_fitness(ind):
        """
        计算适应度
        @params
        ind: tuple 包含一组individual的三个序列
        """
        if ind[0].count(0) != num_blank_a or ind[1].count(0) != num_blank_b or ind[2].count(0) != num_blank_c:
            print("比对序列不合法")
            return -1
        if len(ind[0]) != len(ind[1]) or len(ind[0]) != len(ind[2]):
            print("比对序列不合法")
            return -1
        
        fitness = 0
        index_a = index_b = index_c = 0

        def _calc(a, b, str_a, str_b):
            if a!=0 and b!=0:
                if str_a == str_b:
                    return MISMATCH_COST
                else:
                    return MATCH_COST
            elif a==0 and b==0:
                return MISMATCH_COST
            else:
                return MISMATCH_COST-GAP_COST
            
        for i in range(len(ind[0])):
            # print(index_a, index_b, index_c, len_a, len_b, len_c, ind[0][i], ind[1][i],ind[2][i])
            fitness += _calc(ind[0][i], ind[1][i], query[index_a if index_a < len_a else 0], target1[index_b if index_b < len_b else 0])
            fitness += _calc(ind[0][i], ind[2][i], query[index_a if index_a < len_a else 0], target2[index_c if index_c < len_c else 0])
            fitness += _calc(ind[1][i], ind[2][i], target1[index_b if index_b < len_b else 0], target2[index_c if index_c < len_c else 0])
            index_a += ind[0][i]
            index_b += ind[1][i]
            index_c += ind[2][i]
        return fitness
    
    def calc_cost_and_output(ind):
        """
        计算cost得分并重建字符串结果
        @params
        ind: tuple 包含一组individual的两个序列
        """
        if ind[0].count(0) != num_blank_a or ind[1].count(0) != num_blank_b or ind[2].count(0) != num_blank_c:
            print("比对序列不合法")
            return -1
        if len(ind[0]) != len(ind[1]) or len(ind[0]) != len(ind[2]):
            print("比对序列不合法")
            return -1
        
        cost = 0
        index_a = index_b = index_c = 0
        out_a = ''
        out_b = ''
        out_c = ''

        def _calc(a, b, str_a, str_b):
            if a!=0 and b!=0:
                if str_a == str_b:
                    return MATCH_COST
                else:
                    return MISMATCH_COST
            elif a==0 and b==0:
                return MATCH_COST
            else:
                return GAP_COST
            
        for i in range(len(ind[0])):
            # print(out_a, out_b, out_c)
            if (ind[0][i] == 0 and ind[1][i] == 0 and ind[2][i] == 0): # GAP-GAP-GAP
                continue
            else:
                cost += _calc(ind[0][i], ind[1][i], query[index_a if index_a < len_a else 0], target1[index_b if index_b < len_b else 0])
                cost += _calc(ind[0][i], ind[2][i], query[index_a if index_a < len_a else 0], target2[index_c if index_c < len_c else 0])
                cost += _calc(ind[1][i], ind[2][i], target1[index_b if index_b < len_b else 0], target2[index_c if index_c < len_c else 0])
                out_a += query[index_a] if ind[0][i] == 1 else '-'
                out_b += target1[index_b] if ind[1][i] == 1 else '-'
                out_c += target2[index_c] if ind[2][i] == 1 else '-'
                index_a += ind[0][i]
                index_b += ind[1][i]
                index_c += ind[2][i]
                
        return cost, out_a, out_b, out_c

    def selection(roulette):
        """
        选择
        """
        rand = random.random()
        total = 0

        for i in range(len(roulette)):
            total += roulette[i]
            if rand <= total:
                return i

        return len(roulette) - 1

    def crossover(ind_a, ind_b):
        """
        交叉互换
        @params
        ind_a, ind_b : tuple, tuple 包含两组individual的3个序列
        """   
        pos_cut = random.randint(1, len(ind_a[0]) - 1)
        new_a = (ind_a[0][:pos_cut] + ind_b[0][pos_cut:], ind_a[1][:pos_cut] + ind_b[1][pos_cut:], ind_a[2][:pos_cut] + ind_b[2][pos_cut:])
        new_b = (ind_b[0][:pos_cut] + ind_a[0][pos_cut:], ind_b[1][:pos_cut] + ind_a[1][pos_cut:], ind_b[2][:pos_cut] + ind_a[2][pos_cut:])
        
        def random_adjust(ind, num_blank):
            zeros = [i for i, val in enumerate(ind) if val == 0]  
            ones = [i for i, val in enumerate(ind) if val == 1]  

            if len(zeros) > num_blank:
                # 当0的个数超过num_blank时，随机选择一部分0变为1
                random.shuffle(zeros)
                for i in range(len(zeros) - num_blank):
                    ind[zeros[i]] = 1
            elif len(zeros) < num_blank:
                # 当0的个数小于num_blank时，随机选择一部分1变为0
                random.shuffle(ones)
                for i in range(num_blank - len(zeros)):
                    ind[ones[i]] = 0
            return ind
        
        new_a = (random_adjust(new_a[0], num_blank_a), random_adjust(new_a[1], num_blank_b), random_adjust(new_a[2], num_blank_c))
        new_b = (random_adjust(new_b[0], num_blank_a), random_adjust(new_b[1], num_blank_b), random_adjust(new_b[2], num_blank_c))

        return new_a, new_b

    def mutate_1(lst):
        """
        变异函数一
        @params:
        lst: 单个序列
            随机寻找一对0-1-0和1-0-1, 变为0-0-0和1-1-1
        """
        found_010 = False  
        found_101 = False 
        pos_010 = pos_101 = 0

        # 使用随机采样进行寻找和互换操作
        indices = list(range(len(lst)-2))
        random.shuffle(indices)

        for i in indices:
            if lst[i] == 0 and lst[i+1] == 1 and lst[i+2] == 0 and not found_010:
                pos_010 = i+1
                found_010 = True
            elif lst[i] == 1 and lst[i+1] == 0 and lst[i+2] == 1 and not found_101:
                pos_101 = i+1
                found_101 = True

            if found_010 and found_101:
                break

        if found_010 and found_101:
            lst[pos_010] = 0
            lst[pos_101] = 1

        return lst

    def mutate_2(lst):
        """
        变异函数二
        @params:
        lst: 单个序列
            随机寻找一对0、1, 互换
        """
        zeros = [i for i, val in enumerate(lst) if val == 0]  
        ones = [i for i, val in enumerate(lst) if val == 1]  

        if zeros and ones:
            # 随机选择0和1的位置
            zero_index = random.choice(zeros)
            one_index = random.choice(ones)

            lst[zero_index], lst[one_index] = 1, 0

        return lst
    
    mutate = mutate_2  # 选择变异函数

    # 初始种群
    for _ in range(population_size):
        individual = (generate_seq(len_i, num_blank_a), generate_seq(len_i, num_blank_b), generate_seq(len_i, num_blank_c))
        population.append(individual)

    total_best = population[0]

    # 进化
    for _ in range(generation):
        population = sorted(population, key=calc_fitness, reverse=True)
        fitness = [calc_fitness(individual) for individual in population]
        fitness_sum = sum(fitness)
        roulette = [f/fitness_sum for f in fitness]
        selected_indices = []
        for _ in range(population_size // 2 - 1):
            index_a = selection(roulette)
            index_b = selection(roulette)
            selected_indices.append(index_a)
            selected_indices.append(index_b)
        
        selected_indices.append(0)  # 保送fitness最大的两个个体
        selected_indices.append(1)
        
        best_individual = max(population, key=lambda x: calc_fitness(x))
        if calc_fitness(best_individual) > calc_fitness(total_best):
            total_best = best_individual
        population = [population[i] for i in selected_indices]

        # 交叉
        new_population = []
        for i in range(0, population_size-2, 2):
            parent_a = population[i]
            parent_b = population[i+1]
            child_a, child_b = crossover(parent_a, parent_b)
            new_population.append(child_a)
            new_population.append(child_b)

        # 变异
        for i in range(num_mutate):
            random_index = random.randint(0, len(new_population) - 1)
            individual = new_population[random_index]
            mutated = (mutate(individual[0]), mutate(individual[1]), mutate(individual[2]))
            new_population[random_index] = mutated

        new_population.append(population[-2])  # 保送fitness最大的两个个体
        new_population.append(population[-1])

        population = new_population

    # 返回最优解
    best_individual = max(population, key=lambda x: calc_fitness(x))
    if calc_fitness(best_individual) > calc_fitness(total_best):
        total_best = best_individual
    return calc_cost_and_output(total_best)

if __name__ == "__main__":
    load_dataset()
    expand_len = float(sys.argv[1]) 
    population = int(sys.argv[2])  
    generation = int(sys.argv[3])  
    num_mutate = int(sys.argv[4])
    with open("MSA_query_3.txt") as f:
        for query in f:
            min_cost = float("inf")
            ans_path = ()
            for i, dataset1 in tqdm(enumerate(datasets), desc="Outer loop", total=len(datasets)):
                for j in tqdm(range(i+1, len(datasets)), desc="Inner loop", leave=False):
                    dataset2 = datasets[j]
                    cost, out_a, out_b, out_c = GA(query.replace('\n',''), dataset1.replace('\n',''), dataset2.replace('\n',''), 
                                            expand_len=expand_len, 
                                            population_size=population, 
                                            generation=generation, 
                                            num_mutate=num_mutate)
                    if cost < min_cost:
                        min_cost = cost
                        ans_path = (out_a, out_b, out_c)

            print(min_cost, '\n', ans_path[0], '\n', ans_path[1], '\n', ans_path[2])
