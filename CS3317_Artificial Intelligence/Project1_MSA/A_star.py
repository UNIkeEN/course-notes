from tqdm import tqdm
import numpy as np
import heapq
import time

datasets = []

MATCH_COST = 0
MISMATCH_COST = 5
GAP_COST = 3

class Node:
    def __init__(self, g, path_a, path_b, used_a, used_b):
        self.g = g
        self.used_a = used_a    # 目前使用的序列A字母数
        self.used_b = used_b    # 目前使用的序列B字母数
        self.path_a = path_a    # 目前使用的序列A
        self.path_b = path_b    # 目前使用的序列B

    def __lt__(self, other):
        return self.g < other.g


def load_dataset():
    with open("MSA_database.txt") as f:
        for line in f:
            datasets.append(line)

def A_star(query, target):
    start_node = Node(0, "", "", 0, 0)
    len_a = len(query)
    len_b = len(target)
    visited = np.zeros(shape=(len_a+1, len_b+1),dtype=bool)

    open_heap = []
    # closed_list = set()  

    def h1(node: Node):  # 启发函数
        return GAP_COST * abs((len_a-node.used_a) - (len_b-node.used_b))
    
    def h2(node: Node):  # 启发函数
        remaining_a = query[node.used_a:]
        remaining_b = target[node.used_b:]
        longer_sequence = remaining_a if len(remaining_a) > len(remaining_b) else remaining_b
        shorter_sequence = remaining_a if len(remaining_a) <= len(remaining_b) else remaining_b
        p = 0
        for char in longer_sequence :
            if char not in shorter_sequence:
                p += 1
        return (2 * p - len(longer_sequence)) * GAP_COST if (2 * p - len(longer_sequence)) > 0 else 0
    
    h = h2  # 选择启发函数
    
    heapq.heappush(open_heap, (start_node.g + h(start_node), start_node))

    while open_heap:
        _, current_node = heapq.heappop(open_heap)

        if visited[current_node.used_a][current_node.used_b]:
            continue

        visited[current_node.used_a][current_node.used_b] = True

        # print(len(open_heap))

        # 完成匹配
        if (current_node.used_a == len_a and current_node.used_b == len_b):
            return current_node

        # 生成备选节点
        if (current_node.used_a != len_a):
            new_node = Node(
                current_node.g + GAP_COST, 
                current_node.path_a + query[current_node.used_a],
                current_node.path_b + "-",
                current_node.used_a + 1,
                current_node.used_b
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_b != len_b):
            new_node = Node(
                current_node.g + GAP_COST, 
                current_node.path_a + "-",
                current_node.path_b + target[current_node.used_b],
                current_node.used_a,
                current_node.used_b + 1
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_a != len_a and current_node.used_b != len_b):
            new_node = Node(
                current_node.g + (MISMATCH_COST if query[current_node.used_a] != target[current_node.used_b] else MATCH_COST),
                current_node.path_a + query[current_node.used_a],
                current_node.path_b + target[current_node.used_b],
                current_node.used_a + 1,
                current_node.used_b + 1
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

if __name__ == "__main__":
    load_dataset()
    start_time = time.time()
    with open("MSA_query_2.txt") as f:
        for query in f:
            min_cost = float("inf")
            ans_path = ()
            for dataset in datasets:
                final_node = A_star(query.replace('\n',''), dataset.replace('\n',''),)
                if final_node.g < min_cost:
                    min_cost = final_node.g
                    ans_path = (final_node.path_a, final_node.path_b)   

            print(min_cost, '\n', ans_path[0], '\n', ans_path[1])
    
    end_time = time.time()
    print(end_time - start_time)
    # final_node = A_star("ABCDE", "BBCD")
    # print(final_node.g, final_node.path_a, final_node.path_b)