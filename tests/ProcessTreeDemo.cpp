#include "ProcessTreeDemo.h"

#include <iostream>
#include <vector>

#include <tabulate/table.hpp>

#include "HierNode.h"
#include "Data.h"

using std::cout;
using std::endl;

std::shared_ptr<Hier_Node> build_process_tree(std::vector<std::shared_ptr<Hier_Node>>& children) {
    auto root = std::make_shared<Hier_Node>();
    for (int i = 0; i < 5; ++i) {
        auto child = std::make_shared<Hier_Node>();
        Data d;
        d.cpu_usage = 5.0 + i * 5;
        d.mem_usage = 100.0 + i * 50;
        children.push_back(child);
        root->add_child(child);
        child->set_local_data(d);
    }
    for (auto& child : children) {
        for (int j = 0; j < 2; ++j) {
            auto grandchild = std::make_shared<Hier_Node>();
            Data d;
            d.cpu_usage = 5.0 + j * 2;
            d.mem_usage = 30.0 + j * 10;
            child->add_child(grandchild);
            grandchild->set_local_data(d);
        }
    }
    return root;
}

void print_tree_table(const std::shared_ptr<Hier_Node>& root) {
    tabulate::Table table;
    table.add_row({"PID", "Level", "CPU %", "Memory", "CPU % (local)", "Memory (local)"});
    int pid = 0;
    std::function<void(const std::shared_ptr<Hier_Node>&, int)> add_to_table;
    add_to_table = [&](const std::shared_ptr<Hier_Node>& node, int level) {
        const Data& local_data = node->get_local_data();
        const Data& full_data  = node->get_full_data();
        std::string pid_str;
        if (level == 0) {
            pid_str = "|" + std::to_string(pid++);
        } else {
            pid_str = std::string(level * 2, '.') + "|--" + std::to_string(pid++);
        }
        table.add_row({
            pid_str,
            std::to_string(level),
            std::to_string(full_data.cpu_usage),
            std::to_string(full_data.mem_usage),
            std::to_string(local_data.cpu_usage),
            std::to_string(local_data.mem_usage)
        });
        for (const auto& child : node->get_children()) {
            if (child) add_to_table(child, level + 1);
        }
    };
    add_to_table(root, 0);
    cout << table << endl;
}

void scale_children_cpu(const std::shared_ptr<Hier_Node>& root, double scale) {
    for (auto& child : root->get_children()) {
        Data d = child->get_local_data();
        d.cpu_usage *= scale;
        child->set_local_data(d);
    }
}

void limit_root_cpu(const std::shared_ptr<Hier_Node>& root) {
    double total_cpu = 0.0;
    std::function<void(const std::shared_ptr<Hier_Node>&)> sum_cpu;
    sum_cpu = [&](const std::shared_ptr<Hier_Node>& node) {
        total_cpu += node->get_local_data().cpu_usage;
        for (const auto& child : node->get_children()) {
            if (child) sum_cpu(child);
        }
    };
    sum_cpu(root);
    if (total_cpu > 100.0) {
        double scale = 100.0 / total_cpu;
        scale_children_cpu(root, scale);
        root->update_full();
    }
}

void modify_node_data(const std::shared_ptr<Hier_Node>& node, double cpu, double mem) {
    Data d = node->get_local_data();
    d.cpu_usage = cpu;
    d.mem_usage = mem;
    node->set_local_data(d);
    // 限制根节点 CPU 总和
    // 找到根节点
    std::shared_ptr<Hier_Node> root = node;
    while (root->get_parent().lock()) root = root->get_parent().lock();
    limit_root_cpu(root);
}

void run_process_tree_demo() {
    std::vector<std::shared_ptr<Hier_Node>> children;
    auto root = build_process_tree(children);
    root->update_full();

    cout << "\n初始进程树数据：" << endl;
    print_tree_table(root);

    cout << "\n修改第一个子节点的CPU和内存：" << endl;
    auto first_child = root->get_children().front();
    modify_node_data(first_child, 10.00, 888.88);
    print_tree_table(root);

    cout << "\n修改第一个孙子节点的CPU和内存：" << endl;
    auto first_grandchild = first_child->get_children().front();
    modify_node_data(first_grandchild, 6.20, 666.66);
    print_tree_table(root);

    cout << "\n再次修改第一个子节点的CPU和内存：" << endl;
    modify_node_data(first_child, 8.11, 222.22);
    print_tree_table(root);
}
