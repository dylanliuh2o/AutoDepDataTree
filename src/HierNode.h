#ifndef HIER_NODE_H
#define HIER_NODE_H

#include <memory>
#include <vector>
#include <functional>

#include "Data.h"

class Hier_Node : public std::enable_shared_from_this<Hier_Node> {
public:
    using CallbackFunc = std::function<void(std::shared_ptr<Hier_Node> node)>;

    Hier_Node() = default;
    Hier_Node(const Hier_Node&) = delete;
    Hier_Node& operator=(const Hier_Node&) = delete;

    // Hier_Node manipulation methods
    void add_child(std::shared_ptr<Hier_Node> &child);
    std::vector<std::shared_ptr<Hier_Node>>&       get_children();
    const std::vector<std::shared_ptr<Hier_Node>>& get_children() const;
    std::weak_ptr<Hier_Node>                       get_parent();

    // Propagation methods
    void update_full();

    // Data manipulation methods
    const Data& get_local_data() const;
    const Data& get_full_data() const;
    void        set_local_data(const Data& local_data);

    // void set_update_callback(CallbackFunc cb) { update_callback_ = cb; }

private:
    // CallbackFunc                            update_callback_;
    Data                                    local_data_;
    Data                                    full_data_;
    std::weak_ptr<Hier_Node>                parent_;
    std::vector<std::shared_ptr<Hier_Node>> children_;
} ;

#endif // HIER_NODE_H