#include <iostream>
#include <algorithm>
#include "mpi.h"
#include <bits/stdc++.h>

using namespace std;

const int MAX_NODES = 500;
struct Node
{
    int key;
    int frequency;
};
MPI_Datatype MPI_NODE;
struct computeNode
{
    int cost;
    int root;
};

void merge(vector<Node> &a, vector<Node> &b, int low, int mid, int high)
{
    int i = low, j = mid + 1, h = low;
    while ((h <= mid) && (j <= high))
        b[i++] = (a[h].key <= a[j].key) ? a[h++] : a[j++];

    if (h > mid)
    {
        for (int k = j; k <= high; k++)
            b[i++] = a[k];
    }
    else
    {
        for (int k = h; k <= mid; k++)
            b[i++] = a[k];
    }
    for (int k = low; k <= high; k++)
    {
        a[k] = b[k];
    }
}

void merge_sort(vector<Node> &a, vector<Node> &b, int start, int end)
{
    if (start < end)
    {
        int middle = (start + end) / 2;
        merge_sort(a, b, start, middle);
        merge_sort(a, b, middle + 1, end);
        merge(a, b, start, middle, end);
    }
}

void parallelMergeSort(vector<Node> &nodes, int num_procs)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = nodes.size();
    int block_size = size / num_procs;
    int start = rank * block_size;
    int end = (rank + 1) * block_size;
    if (rank == num_procs - 1)
    {
        end = size - 1;
        block_size = end - start + 1;
    }
    vector<int> recvcounts(num_procs);
    vector<int> displs(num_procs);
    int sz = block_size;
    MPI_Gather(&sz, 1, MPI_INT, &recvcounts[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        int total = accumulate(recvcounts.begin(), recvcounts.end(), 0);
        displs[0] = 0;
        for (int i = 1; i < num_procs; i++)
            displs[i] = displs[i - 1] + recvcounts[i - 1];
    }

    vector<Node> sub_array(sz);
    MPI_Scatterv(&nodes[0], &recvcounts[0], &displs[0], MPI_NODE, &sub_array[0], sz, MPI_NODE, 0, MPI_COMM_WORLD);
    vector<Node> tmp_array(block_size);

    merge_sort(sub_array, tmp_array, 0, block_size - 1);

    vector<Node> gathered_array(0);
    if (rank == 0)
    {
        int totNodes = accumulate(recvcounts.begin(), recvcounts.end(), 0);
        gathered_array.resize(totNodes);
    }
    MPI_Gatherv(&sub_array[0], sz, MPI_NODE, &gathered_array[0], &recvcounts[0], &displs[0], MPI_NODE, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int totNodes = accumulate(recvcounts.begin(), recvcounts.end(), 0);
        vector<Node> other_array(totNodes);
        merge_sort(gathered_array, other_array, 0, totNodes - 1);
        other_array.clear();
    }
    if (rank == 0)
    {
        for (int i = 0; i < gathered_array.size(); i++)
            nodes[i] = gathered_array[i];
    }
    gathered_array.clear();
    sub_array.clear();
    tmp_array.clear();
}

void compute_cost(vector<Node> &nodes, int num_nodes, vector<int> &prefSum)
{

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int highBlock;

    int sz = (num_nodes + 3);
    struct computeNode **cnode, *tnode;
    cnode = new struct computeNode *[sz];
    tnode = new struct computeNode[sz];
    cnode[0] = new struct computeNode[sz * sz];
    for (int i = 1; i < sz; i++)
        cnode[i] = cnode[i - 1] + sz;

    for (int i = 0; i < sz; i++)
        for (int j = 0; j < sz; j++)
        {
            cnode[i][j].cost = INT_MAX;
            cnode[i][j].root = -1;
        }

    for (int i = 1; i <= num_nodes; i++)
    {
        cnode[1][i].cost = nodes[i - 1].frequency;
        cnode[1][i].root = i;
    }
    highBlock = num_nodes - 1;
    for (int dlen = 2; dlen <= num_nodes; dlen++)
    {
        int blow = (rank * highBlock) / num_procs;
        int bhigh = (((rank + 1) * highBlock) / num_procs) - 1;
        if (bhigh - blow + 1 > 0)
        {
            for (int j = dlen + blow; j <= dlen + bhigh and j <= num_nodes; j++)
            {
                int rootDiag = cnode[dlen - 1][j - 1].root, rootAbove = cnode[dlen - 1][j].root;
                int diag_row = j - dlen + 1, diag_col = j;
                int nDiag = rootAbove - rootDiag + 1;
                int sLeft = (dlen) - (rootDiag - diag_row);
                int sRight = (rootDiag - diag_row) + 1;
                int tmp = INT_MAX;
                for (int l = 1; l < nDiag + 1; l++)
                {
                    int leftCost = (dlen - sLeft > 0 && j - sLeft > 0) ? cnode[dlen - sLeft][j - sLeft].cost : 0;
                    int rightCost = (dlen - sRight > 0) ? cnode[dlen - sRight][j].cost : 0;
                    int tcost = leftCost + rightCost;
                    if (tcost < tmp)
                    {
                        tmp = tcost;
                        cnode[dlen][j].root = cnode[dlen - 1][j - 1].root + l - 1;
                    }
                    sLeft--;
                    sRight++;
                }
                cnode[dlen][j].cost = prefSum[diag_col] - prefSum[diag_row - 1] + tmp;
            }
        }
        MPI_Allreduce(&(cnode[dlen][dlen]), tnode, highBlock, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
        for (int k = 0; k < highBlock; k++)
            cnode[dlen][dlen + k] = tnode[k];
        highBlock--;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        // ofstream fout("out.txt");
        cout << cnode[num_nodes][num_nodes].cost << endl;
        vector<vector<int>> roots(num_nodes + 1, vector<int>(num_nodes + 1, -1));
        int x = 0;
        for (int i = 1; i <= num_nodes; i++)
        {
            for (int j = 1; j <= num_nodes; j++)
            {
                if (j + x <= num_nodes)
                    roots[j][j + x] = cnode[i][j + x].root;
            }
            x++;
        }
        vector<int> parent(num_nodes + 1, -1);
        function<void(int, int, int)> findParent = [&](int i, int j, int par)
        {
            if (i > j)
                return;
            int cur_root = roots[i][j];
            parent[cur_root] = par;
            findParent(i, cur_root - 1, cur_root);
            findParent(cur_root + 1, j, cur_root);
        };
        findParent(1, num_nodes, -1);
        for (int i = 1; i <= num_nodes; i++)
        {
            if (parent[i] == -1)
            {
                parent[i] = 0;
            }
            cout << parent[i] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    int num_nodes;
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    ifstream fin("output.txt");

    if (rank == 0)
        cin >> num_nodes;
    MPI_Bcast(&num_nodes, 1, MPI_INT, 0, MPI_COMM_WORLD);
    vector<Node> nodes(num_nodes);
    if (rank == 0)
    {
        for (int i = 0; i < num_nodes; i++)
        {
            cin >> nodes[i].key >> nodes[i].frequency;
        }
    }
    MPI_Type_contiguous(2, MPI_INT, &MPI_NODE);
    MPI_Type_commit(&MPI_NODE);
    double start = MPI_Wtime();
    parallelMergeSort(nodes, num_procs);
    MPI_Bcast(&nodes[0], num_nodes, MPI_NODE, 0, MPI_COMM_WORLD);

    vector<int> prefSum(num_nodes + 1, 0);
    if (rank == 0)
    {
        for (int i = 1; i <= num_nodes; i++)
        {
            prefSum[i] = prefSum[i - 1] + nodes[i - 1].frequency;
        }
    }
    MPI_Bcast(&prefSum[0], num_nodes + 1, MPI_INT, 0, MPI_COMM_WORLD);
    compute_cost(nodes, num_nodes, prefSum);

    MPI_Finalize();
    return 0;
}
