from tqdm import tqdm
import numpy as np
import time

datasets = []

MATCH_COST = 0
MISMATCH_COST = 5
GAP_COST = 3

def load_dataset():
    with open("MSA_database.txt") as f:
        for line in f:
            datasets.append(line)

def DP(query, target):
    len_a, len_b = len(query), len(target)

    c = np.zeros((len_a+1, len_b+1), dtype=int)

    for i in range(len_a, -1, -1):
        c[i][len_b] = GAP_COST * (len_a - i)
    for j in range(len_b, -1, -1):
        c[len_a][j] = GAP_COST * (len_b - j)

    for i in range(len_a-1, -1, -1):
        for j in range(len_b-1, -1, -1):
            match_move = c[i+1][j+1] + (MATCH_COST if query[i] == target[j] else MISMATCH_COST)
            gap_query_move = c[i+1][j] + GAP_COST
            gap_target_move = c[i][j+1] + GAP_COST

            c[i][j] = min(match_move, gap_query_move, gap_target_move)

    # 生成字符串结果
    i, j = 0, 0
    aligned_query, aligned_target = "", ""

    while i < len_a or j < len_b:
        if i < len_a and j < len_b and c[i][j] == c[i+1][j+1] + (MATCH_COST if query[i] == target[j] else MISMATCH_COST):
            aligned_query += query[i]
            aligned_target += target[j]
            i += 1
            j += 1
        elif i < len_a and c[i][j] == c[i+1][j] + GAP_COST:
            aligned_query += query[i]
            aligned_target += '-'
            i += 1
        else:
            aligned_query += '-'
            aligned_target += target[j]
            j += 1

    return aligned_query, aligned_target, c


if __name__ == "__main__":
    load_dataset()
    start_time = time.time()
    with open("MSA_query_2.txt") as f:
        for query in f:
            min_cost = float("inf")
            ans_path = ()
            for dataset in datasets:
                path_a, path_b, cost = DP(query.replace('\n',''), dataset.replace('\n',''))
                if cost[0][0] < min_cost:
                    min_cost = cost[0][0]
                    ans_path = (path_a, path_b)

            print(min_cost, '\n', ans_path[0], '\n', ans_path[1])

    end_time = time.time()
    print(end_time - start_time)
    # final_node = A_star("ABCDE", "BBCD")
    # print(final_node.g, final_node.path_a, final_node.path_b)