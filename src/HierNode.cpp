#include <memory>
#include <algorithm>

#include "Data.h"
#include "HierNode.h"

void Hier_Node::add_child(std::shared_ptr<Hier_Node> &child) {
    if (child) {
        children_.push_back(child);
        child->parent_ = shared_from_this();
        update_full();
    }
}

std::vector<std::shared_ptr<Hier_Node>>& Hier_Node::get_children() {
    return children_;
}

const std::vector<std::shared_ptr<Hier_Node>>& Hier_Node::get_children() const {
    return children_;
}

std::weak_ptr<Hier_Node> Hier_Node::get_parent() {
    return parent_;
}

void Hier_Node::update_full() {
    full_data_.clear();

    // merge local data and children data to full_data_
    full_data_.merge(local_data_);
    for (const auto& child : children_) {
        if (child) {
            full_data_.merge(child->get_full_data());
        }
    }

    // upward propagation
    if (auto parent = parent_.lock()) {
        parent->update_full();
    }

    // if (update_callback_) {
    //     update_callback_(shared_from_this());
    // }
}

const Data& Hier_Node::get_local_data() const {
    return local_data_;
}

const Data& Hier_Node::get_full_data() const {
    return full_data_;
}

void Hier_Node::set_local_data(const Data& local_data) {
    local_data_ = local_data;
    update_full();
}