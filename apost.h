#ifndef APOST_H
#define APOST_H

#include "interval.h"

#include <functional>
#include <utility>
#include <vector>
#include <iostream>

/*
    This file contains all necessary code for Aposteriori interval
    computations error improvement.
    
    Controller class - class for intermediate computation results storage and
        final error computation.
        
    ProxyInterval - wrapper class at intervals. Adds necessary commands  to
        controller during the computation. It has {+,-,*,/) operations.
        
    ProxyIntervalResult - class for output values error computation. Can
        be used for programs with many output values.
        
    For a more detailed description, see:
        [1] Matijasevich, Yu., A posteriori interval analysis, Springer-Verlag,
            Lecture Notes in Computer Science., Volume 204, 1985, pp. 328-334
        [2] Matijasevich, Yu., Real Numbers and Computers, Cybernetics and
            Computing Machinery, 2, 1986, pp. 104-133. (In Russian).

*/

namespace interval {

namespace apost {

// Debug value can be set in user program.
// If true shows error computation process.
static bool debug = false;

// TODO : 
//  1) controller should be singleton;
//  2) faster implementation of proxy object (there are at least extra call of 
//      arithmetic operations in {+,-,*,/} operators);
//  3) ProxyIntervalResult operator= maybe should not copy controller data;
//  4) !!! remove interaction of user with controller class;
//  5) faster implementation of (corr, null, inull) commands.

/*
    Controller class for apost error improvement method.
    There is a static object of Controller class named "controller".
    It must be performed in the following order:
        1) set input ProxyInterval variables;
        2) call controller.init();
        3) do computations using ProxyInterval objects for temporal variables;
        4) call controller.evaluate() to get an apost computed error.
    - step 4) can be performed only once
    
    
    ProxyIntervalResult can be used for programs with multiple output values.
    It must be performed in the following order:
        1) set input ProxyInterval variables;
        2) call controller.init();
        3) do computations using ProxyInterval objects for temporal variables;
        4) set output value ProxyIntervalResult y = "some computations".
            value y now contains improved error and can be converted to
            simple interval.
*/
template<class IntervalT>
class Controller {
public:
    // Initializes the controller. 
    // Must be called after all input variables 
    // setting and before computations.
    void init() {
        size_t last = memory_.size() - 1;
    
        // pushes
        // 1.      corr 0 (1, 0)
        //         inull 1
        //         corr 1 (1, 0)
        //         inull 2
        //         ...
        //         corr (last - 1) (1, 0)
        // 2.      inull last
        //         corr last (err of last x, 0)
        //         inull last
        // 3.      ...
        //         corr 0 (err of first x, 0)
        //         inull 0
        // to the controller commands list.
        
        // 1.
        push_corr(0, 1);
        for (size_t i = 1; i < last; ++i) {
            push_inull(i);
            push_corr(i, 1);
        }
        
        // 2.
        push_inull(last);
        push_corr(last, IntervalT(memory_[last].error()));
        push_inull(last);
        
        // 3.
        for (size_t i = 1; i <= last; ++i) {
            push_corr(last - i, IntervalT(memory_[last - i].error()));
            push_inull(last - i);
        }
    }
    
    // Computes and returns the error of last ProxyInterval
    // arithmetic operation result.
    // Clears Controller data after computtation.
    IntervalT evaluate() {
        size_t last = commands_.size() - 1;
        IntervalT result = memory_.back();      // last computed value
        
        // set all memory_ to 0 and last element to 1
        for (auto &i : memory_) {
            i = 0;
        }
        memory_.back() = 1;
        
        // call saved commands in reverse order
        for (size_t i = 0; i <= last; ++i) {
            commands_[last - i]();
        }
        
        // result error contains in first memory_ element
        result = IntervalT(result.val(), memory_[0].val() + memory_[0].error());
        
        // clear all
        memory_.clear();
        commands_.clear();
       
        return result;
    }

    // Pushes new interval value to Controller memory and returns its address.
    size_t push_value(const IntervalT& value) {
        memory_.push_back(value);
        return memory_.size() - 1;
    }

    // Pushes memory_[a] + memory_[b] to Controller and returns
    // it's address.
    
    // pushes
    // corresponding to "+" controller operations:
    //         corr b (1, 0)
    //         corr a (1, 0)
    //         null last
    size_t add(size_t a, size_t b) {
        memory_.push_back(memory_[a] + memory_[b]);
        size_t last = memory_.size() - 1;
        
        push_corr(b, 1);
        push_corr(a, 1);
        push_null(last);
        
        return last;
    }

    // Pushes memory_[a] - memory_[b] to Controller and returns
    // it's address.
    
    // pushes
    // corresponding to "-" controller operations:
    //         corr b (-1, 0)
    //         corr a (1, 0)
    //         null last
    size_t sub(size_t a, size_t b) {
        memory_.push_back(memory_[a] - memory_[b]);
        size_t last = memory_.size() - 1;
        
        push_corr(b, -1);
        push_corr(a, 1);
        push_null(last);
        
        return last;
    }

    // Pushes memory_[a] * memory_[b] to Controller and returns
    // it's address.
    
    // pushes
    // corresponding to "*" controller operations:
    //         corr b (memory_[a], 0)
    //         corr a (memory_[b], 0)
    //         null last
    size_t mul(size_t a, size_t b) {
        memory_.push_back(memory_[a] * memory_[b]);
        size_t last = memory_.size() - 1;
        
        push_corr(b, memory_[a]);
        push_corr(a, memory_[b]);
        push_null(last);
        
        return last;
    }

    // Pushes memory_[a] - memory_[b] to Controller and returns
    // it's address.
    
    // pushes
    // corresponding to "/" controller operations:
    //         corr b ( -memory_[a] / memory_[b] / memor_[b] )
    //         corr a ( (1, 0) / memory_[b] )
    //         null last
    size_t div(size_t a, size_t b) {
        memory_.push_back(memory_[a] / memory_[b]);
        
        size_t last = memory_.size() - 1;
        
        // temp = -memory_[a] / memory_[b] / memor_[b]
        IntervalT temp = -memory_[a];
        temp = temp / memory_[b];
        temp = temp / memory_[b];
        
        push_corr(b, temp);
        push_corr(a, IntervalT(1) / memory_[b]);
        push_null(last);
        
        return last;
    }
    
    // Prints the memory content.
    void print() const {
        for (auto x : memory_) {
            std::cout << x << std::endl;
        }
    }
    
    // Assignment operator uses in ProxyIntervalResult.
    Controller& operator=(Controller other) {
        swap(memory_, other.memory_);
        swap(commands_, other.commands_);
        
        return *this;
    }
    
private:
    /*
        Controller class uses 3 simple commands and s_ aggregation value
        to compute error.
        
        
        Commands:
        corr "addr" "interval"
            sets memory_[addr] = memory_[addr] + interval * s_
            
        null "addr"
            sets s_ = memory_[addr] and memory_[addr] = 0
    
        inull "addr"
            sets s_ = abs(memory_[addr]) and memory_[addr] = 0
            
        
        This comands stores in commands_ vector during computation using
        push_corr, push_null and push_inull methods.
        Finally in evaluate methods pushed functions calls in reverse order
        and computed error is in memory_[0].
    */

    std::vector<IntervalT> memory_;
    std::vector<std::function<void()>> commands_;
    
    IntervalT s_;
    
    // Pushes corr command to commands vector.
    void push_corr(size_t a, IntervalT x) {
        commands_.push_back( [a, x, this] {
            if (debug) 
                std::cerr << "corr: " << a << " " << x << " " << s_ << std::endl;
            memory_[a] += x * s_;
        } );
    }
    
    // Pushes null command to commands vector.
    void push_null(size_t a) {
        commands_.push_back( [a, this] {
            if (debug)
                std::cerr << "null: " << a <<  " " << s_ << std::endl;
            s_ = memory_[a];
            memory_[a].zero(); 
        } );
    }
    
    // Pushes inull command to commands vector.
    void push_inull(size_t a) {
        commands_.push_back( [a, this] {
            if (debug)
                std::cerr << "inull: " << a <<  " " << s_ << std::endl;
            memory_[a].abs();
            s_ = memory_[a];
            memory_[a].zero(); 
        } );
    }
};

static Controller<ArbInterval> controller;


// Proxy class for apost interval computations.
// In addition to traditional computing adds reverse commands to controller.
template<class IntervalT>
struct ProxyInterval {
    // Constructors
    ProxyInterval() {
        data_.zero();
        addr_ = 0;
    }
    
    // Constructor to init ProxyInterval from simple interval.
    // New object has the new address in controller object.
    ProxyInterval(const IntervalT& other) {
        data_ = other;
        addr_ = controller.push_value(other);
    }
    
    // Constructor to init ProxyInterval from other ProxyInterval.
    // New object has the same address as other in controller object.
    ProxyInterval& operator=(const ProxyInterval& other) {
        data_ = other.data_;
        addr_ = other.addr_;
    }
    
    // Simple interval cast operator.
    operator IntervalT() const { return data_; }

    // ProxyInterval arithmetical operations computes the corresponding
    // value and calls controller method to add this value to memory and
    // push required inversion operations.
    
    // Returns *this + other.
    ProxyInterval operator+(const ProxyInterval& other) {
        ProxyInterval temp;
        
        temp.data_ = data_ + other.data_;
        temp.addr_ = controller.add(addr_, other.addr_);
        
        return temp;
    }
    
    // Returns *this - other.
    ProxyInterval operator-(const ProxyInterval& other) {
        ProxyInterval temp;
        
        temp.data_ = data_ - other.data_;
        temp.addr_ = controller.sub(addr_, other.addr_);
        
        return temp;
    }
    
    // Returns -*this.
    ProxyInterval operator-() {
        ProxyInterval temp;
        temp = ProxyInterval(ArbInterval(0)) - *this;
        return temp;
    }
    
    // Returns *this * other.
    ProxyInterval operator*(const ProxyInterval& other) {
        ProxyInterval temp;
        
        temp.data_ = data_ * other.data_;
        temp.addr_ = controller.mul(addr_, other.addr_);
        
        return temp;
    }
    
    // Returns *this / other.
    ProxyInterval operator/(const ProxyInterval& other) {
        ProxyInterval temp;
        
        temp.data_ = data_ / other.data_;
        temp.addr_ = controller.div(addr_, other.addr_);
        
        return temp;
    }
    
    // Returns internal ProxyInterval data.
    IntervalT data() const { return data_; }
    
private:
    IntervalT data_;
    size_t addr_;
};


// Proxy object for result value. Performs all computations for 
// error improvement of equated value.
class ProxyIntervalResult {
public:

    // Sets ProxyIntervalResult = evaluated value of ProxyInterval<ArbInterval>
    ProxyIntervalResult& operator=(const ProxyInterval<ArbInterval>& other) {
        Controller<ArbInterval> temp = controller;
        result_ = controller.evaluate();
        controller = temp;
        
        data_ = other.data();
        
        return *this;
    }
    
    ProxyIntervalResult& operator=(const ProxyIntervalResult& other) {
        result_ = other.result_;
        data_ = other.data_;
        
        return *this;
    }
    
    // Simple interval (ArbInterval) cast operator.
    operator ArbInterval() const {
        if (result_.error() > data_.error()) return data_;
        return result_;
    }
    
private:
    ArbInterval result_;
    ArbInterval data_;
};

}  // namespace apost

}  // namespace interval


#endif  // APOST_H

