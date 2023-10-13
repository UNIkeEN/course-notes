from tqdm import tqdm
import numpy as np
import heapq
import DP

datasets = []

MATCH_COST = 0
MISMATCH_COST = 5
GAP_COST = 3

class Node:
    def __init__(self, g, path_a, path_b, path_c, used_a, used_b, used_c):
        self.g = g
        self.used_a = used_a
        self.used_b = used_b 
        self.used_c = used_c
        self.path_a = path_a 
        self.path_b = path_b   
        self.path_c = path_c   

    def __lt__(self, other):
        return self.g < other.g

def load_dataset():
    with open("MSA_database.txt") as f:
        for line in f:
            datasets.append(line)

def A_star(query, target1, target2, cur_min_cost):
    start_node = Node(0, "", "", "", 0, 0 ,0)
    len_a = len(query)
    len_b = len(target1)
    len_c = len(target2)
    visited = np.zeros(shape=(len_a+1, len_b+1, len_c+1),dtype=bool)

    # 两两计算动归
    _, __, dp_cost1 = DP.DP(query, target1)
    _, __, dp_cost2 = DP.DP(query, target2)
    _, __, dp_cost3 = DP.DP(target1, target2)

    #  LBE 下界估计 剪枝
    if dp_cost1[0][0] + dp_cost2[0][0] + dp_cost3[0][0] > cur_min_cost:
        return Node(cur_min_cost, "", "", "", 0, 0 ,0)

    open_heap = []

    def h1(node: Node):  # 启发函数一
        return GAP_COST * (abs((len_a-node.used_a) - (len_b-node.used_b)) + abs((len_a-node.used_a) - (len_c-node.used_c)) + abs((len_b-node.used_b) - (len_c-node.used_c)))
    
    def h2(node: Node):  # 启发函数二
        return dp_cost1[node.used_a][node.used_b] + dp_cost2[node.used_a][node.used_c] + dp_cost3[node.used_b][node.used_c]
    
    h = h2  # 选择启发函数
    
    heapq.heappush(open_heap, (start_node.g + h(start_node), start_node))

    while open_heap:
        _, current_node = heapq.heappop(open_heap)

        if visited[current_node.used_a][current_node.used_b][current_node.used_c]:
            continue

        visited[current_node.used_a][current_node.used_b][current_node.used_c] = True

        # 完成匹配
        if (current_node.used_a == len_a and current_node.used_b == len_b and current_node.used_c == len_c):
            return current_node

        # 生成备选节点
        if (current_node.used_a != len_a):
            new_node = Node(
                current_node.g + GAP_COST * 2, 
                current_node.path_a + query[current_node.used_a],
                current_node.path_b + "-",
                current_node.path_c + "-",
                current_node.used_a + 1,
                current_node.used_b,
                current_node.used_c
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_b != len_b):
            new_node = Node(
                current_node.g + GAP_COST * 2, 
                current_node.path_a + "-",
                current_node.path_b + target1[current_node.used_b],
                current_node.path_c + "-",
                current_node.used_a,
                current_node.used_b + 1,
                current_node.used_c,
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_c != len_c):
            new_node = Node(
                current_node.g + GAP_COST * 2, 
                current_node.path_a + "-",
                current_node.path_b + "-",
                current_node.path_c + target2[current_node.used_c],
                current_node.used_a,
                current_node.used_b,
                current_node.used_c + 1,
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_a != len_a and current_node.used_b != len_b):
            new_node = Node(
                current_node.g + (MISMATCH_COST if query[current_node.used_a] != target1[current_node.used_b] else MATCH_COST) + GAP_COST * 2,
                current_node.path_a + query[current_node.used_a],
                current_node.path_b + target1[current_node.used_b],
                current_node.path_c + "-",
                current_node.used_a + 1,
                current_node.used_b + 1,
                current_node.used_c
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_a != len_a and current_node.used_c != len_c):
            new_node = Node(
                current_node.g + (MISMATCH_COST if query[current_node.used_a] != target2[current_node.used_c] else MATCH_COST) + GAP_COST * 2,
                current_node.path_a + query[current_node.used_a],
                current_node.path_b +  "-",
                current_node.path_c + target2[current_node.used_c],
                current_node.used_a + 1,
                current_node.used_b,
                current_node.used_c + 1
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_b != len_b and current_node.used_c != len_c):
            new_node = Node(
                current_node.g + (MISMATCH_COST if target1[current_node.used_b] != target2[current_node.used_c] else MATCH_COST) + GAP_COST * 2,
                current_node.path_a + "-",
                current_node.path_b + target1[current_node.used_b],
                current_node.path_c + target2[current_node.used_c],
                current_node.used_a,
                current_node.used_b + 1,
                current_node.used_c + 1
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

        if (current_node.used_a != len_a and current_node.used_b != len_b and current_node.used_c != len_c):
            new_node = Node(
                current_node.g + ((MISMATCH_COST if query[current_node.used_a] != target1[current_node.used_b] else MATCH_COST) + (MISMATCH_COST if query[current_node.used_a] != target2[current_node.used_c] else MATCH_COST) + (MISMATCH_COST if target1[current_node.used_b] != target2[current_node.used_c] else MATCH_COST)) ,
                current_node.path_a + query[current_node.used_a],
                current_node.path_b + target1[current_node.used_b],
                current_node.path_c + target2[current_node.used_c],
                current_node.used_a + 1,
                current_node.used_b + 1,
                current_node.used_c + 1
            )
            heapq.heappush(open_heap, (new_node.g + h(new_node), new_node))

if __name__ == "__main__":
    load_dataset()
    with open("MSA_query_3.txt") as f:
        for query in f:
            min_cost = float("inf")
            ans_path = ()
            for i, dataset1 in tqdm(enumerate(datasets), desc="Outer loop", total=len(datasets)):
                for j in tqdm(range(i+1, len(datasets)), desc="Inner loop", leave=False):
                    dataset2 = datasets[j]
                    final_node = A_star(query.replace('\n',''), dataset1.replace('\n',''), dataset2.replace('\n',''), min_cost)
                    if final_node.g < min_cost:
                        min_cost = final_node.g
                        ans_path = (final_node.path_a, final_node.path_b, final_node.path_c)

            print(min_cost, '\n', ans_path[0], '\n', ans_path[1], '\n', ans_path[2])

    # final_node = A_star("ABCDE", "BBCD")
    # print(final_node.g, final_node.path_a, final_node.path_b)