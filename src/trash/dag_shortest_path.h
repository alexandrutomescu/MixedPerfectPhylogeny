/* -*- C++ -*-
 *
 * This file is a part of LEMON, a generic C++ optimization library
 *
 * Copyright (C) 2003-2008
 * Egervary Jeno Kombinatorikus Optimalizalasi Kutatocsoport
 * (Egervary Research Group on Combinatorial Optimization, EGRES).
 *
 * Permission to use, modify and distribute this software is granted
 * provided that this copyright notice appears in all copies. For
 * precise terms see the accompanying LICENSE file.
 *
 * This software is provided "AS IS" with no warranty of any kind,
 * express or implied, and with no claim as to its suitability for any
 * purpose.
 *
 */

#ifndef LEMON_DAG_SHORTEST_PATH_H
#define LEMON_DAG_SHORTEST_PATH_H

///\ingroup flowalgs
/// \file
/// \brief DagShortestPath algorithm.
///

#include <lemon/error.h>
#include <lemon/maps.h>

#include <limits>

namespace lemon {

  /// \brief Default OperationTraits for the DagShortestPath algorithm class.
  ///  
  /// It defines all computational operations and constants which are
  /// used in the dag shortest path algorithm. The default implementation
  /// is based on the numeric_limits class. If the numeric type does not
  /// have infinity value then the maximum value is used as extremal
  /// infinity value.
  template <
    typename Value, 
    bool has_infinity = std::numeric_limits<Value>::has_infinity>
  struct DagShortestPathDefaultOperationTraits {
    /// \brief Gives back the zero value of the type.
    static Value zero() {
      return static_cast<Value>(0);
    }
    /// \brief Gives back the positive infinity value of the type.
    static Value infinity() {
      return std::numeric_limits<Value>::infinity();
    }
    /// \brief Gives back the sum of the given two elements.
    static Value plus(const Value& left, const Value& right) {
      return left + right;
    }
    /// \brief Gives back true only if the first value less than the second.
    static bool less(const Value& left, const Value& right) {
      return left < right;
    }
  };

  template <typename Value>
  struct DagShortestPathDefaultOperationTraits<Value, false> {
    static Value zero() {
      return static_cast<Value>(0);
    }
    static Value infinity() {
      return std::numeric_limits<Value>::max();
    }
    static Value plus(const Value& left, const Value& right) {
      if (left == infinity() || right == infinity()) return infinity();
      return left + right;
    }
    static bool less(const Value& left, const Value& right) {
      return left < right;
    }
  };
  
  /// \brief Default traits class of DagShortestPath class.
  ///
  /// Default traits class of DagShortestPath class.
  /// \param _Graph Graph type.
  /// \param _LegthMap Type of length map.
  template<class _Graph, class _LengthMap>
  struct DagShortestPathDefaultTraits {
    /// The graph type the algorithm runs on. 
    typedef _Graph Graph;

    /// \brief The type of the map that stores the arc lengths.
    ///
    /// The type of the map that stores the arc lengths.
    /// It must meet the \ref concepts::ReadMap "ReadMap" concept.
    typedef _LengthMap LengthMap;

    // The type of the length of the arcs.
    typedef typename _LengthMap::Value Value;

    /// \brief Operation traits for dag shortest path algorithm.
    ///
    /// It defines the infinity type on the given Value type
    /// and the used operation.
    /// \see DagShortestPathDefaultOperationTraits
    typedef DagShortestPathDefaultOperationTraits<Value> OperationTraits;
 
    /// \brief The type of the map that stores the last arcs of the 
    /// shortest paths.
    /// 
    /// The type of the map that stores the last
    /// arcs of the shortest paths.
    /// It must meet the \ref concepts::WriteMap "WriteMap" concept.
    ///
    typedef typename Graph::template NodeMap<typename _Graph::Arc> PredMap;

    /// \brief Instantiates a PredMap.
    /// 
    /// This function instantiates a \ref PredMap. 
    /// \param graph is the graph, to which we would
    /// like to define the PredMap.
    /// \todo The graph alone may be insufficient for the initialization
    static PredMap *createPredMap(const _Graph& graph) {
      return new PredMap(graph);
    }

    /// \brief The type of the map that stores the dists of the nodes.
    ///
    /// The type of the map that stores the dists of the nodes.
    /// It must meet the \ref concepts::WriteMap "WriteMap" concept.
    ///
    typedef typename Graph::template NodeMap<typename _LengthMap::Value> 
    DistMap;

    /// \brief Instantiates a DistMap.
    ///
    /// This function instantiates a \ref DistMap. 
    /// \param graph is the graph, to which we would like to define the 
    /// \ref DistMap
    static DistMap *createDistMap(const _Graph& graph) {
      return new DistMap(graph);
    }

  };
  
  /// \brief Inverse OperationTraits for the DagShortestPath algorithm class.
  /// 
  /// It defines all computational operations and constants which are
  /// used in the dag shortest path algorithm. It is the inverse of
  /// \ref DagShortestPathDefaultOperationTraits, so it can be used to
  /// calculate the longest path. The default implementation
  /// is based on the numeric_limits class. If the numeric type does not
  /// have infinity value then the minimum value is used as extremal
  /// infinity value.
  template <
    typename Value, 
    bool has_infinity = std::numeric_limits<Value>::has_infinity>
  struct DagLongestPathOperationTraits {
    /// \brief Gives back the zero value of the type.
    static Value zero() {
      return static_cast<Value>(0);
    }
    /// \brief Gives back the negative infinity value of the type.
    static Value infinity() {
      return -(std::numeric_limits<Value>::infinity());
    }
    /// \brief Gives back the sum of the given two elements.
    static Value plus(const Value& left, const Value& right) {
      return left + right;
    }
    /// \brief Gives back true only if the first value less than the second.
    static bool less(const Value& left, const Value& right) {
      return left > right;
    }
  };

  template <typename Value>
  struct DagLongestPathOperationTraits<Value, false> {
    static Value zero() {
      return static_cast<Value>(0);
    }
    static Value infinity() {
      return std::numeric_limits<Value>::min();
    }
    static Value plus(const Value& left, const Value& right) {
      if (left == infinity() || right == infinity()) return infinity();
      return left + right;
    }
    static bool less(const Value& left, const Value& right) {
      return left > right;
    }
  };

  /// \brief Inverse traits class of DagShortestPath class.
  ///
  /// Inverse traits class of DagShortestPath class.
  /// \param _Graph Graph type.
  /// \param _LegthMap Type of length map.
  template<class _Graph, class _LengthMap>
  struct DagLongestPathTraits {
    /// The graph type the algorithm runs on. 
    typedef _Graph Graph;

    /// \brief The type of the map that stores the arc lengths.
    ///
    /// The type of the map that stores the arc lengths.
    /// It must meet the \ref concepts::ReadMap "ReadMap" concept.
    typedef _LengthMap LengthMap;

    // The type of the length of the arcs.
    typedef typename _LengthMap::Value Value;

    /// \brief Inverse operation traits for dag shortest path algorithm.
    ///
    /// It defines the infinity type on the given Value type
    /// and the used operation.
    /// \see DagLongestPathOperationTraits
    typedef DagLongestPathOperationTraits<Value> OperationTraits;
 
    /// \brief The type of the map that stores the last arcs of the 
    /// longest paths.
    /// 
    /// The type of the map that stores the last
    /// arcs of the longest paths.
    /// It must meet the \ref concepts::WriteMap "WriteMap" concept.
    ///
    typedef typename Graph::template NodeMap<typename _Graph::Arc> PredMap;

    /// \brief Instantiates a PredMap.
    /// 
    /// This function instantiates a \ref PredMap. 
    /// \param graph is the graph,
    /// to which we would like to define the PredMap.
    /// \todo The graph alone may be insufficient for the initialization
    static PredMap *createPredMap(const _Graph& graph) {
      return new PredMap(graph);
    }

    /// \brief The type of the map that stores the dists of the nodes.
    ///
    /// The type of the map that stores the dists of the nodes.
    /// It must meet the \ref concepts::WriteMap "WriteMap" concept.
    ///
    typedef typename Graph::template NodeMap<typename _LengthMap::Value> 
    DistMap;

    /// \brief Instantiates a DistMap.
    ///
    /// This function instantiates a \ref DistMap. 
    /// \param graph is the graph, to which we would like to define the 
    /// \ref DistMap
    static DistMap *createDistMap(const _Graph& graph) {
      return new DistMap(graph);
    }

  };
  

  /// \brief %DagShortestPath algorithm class.
  ///
  /// \ingroup flowalgs
  /// This class provides an efficient implementation of a Dag sortest path
  /// searching algorithm. The arc lengths are passed to the algorithm
  /// using a \ref concepts::ReadMap "ReadMap", so it is easy to change it
  /// to any kind of length.
  ///
  /// The complexity of the algorithm is O(n + e).
  ///
  /// The type of the length is determined by the
  /// \ref concepts::ReadMap::Value "Value" of the length map.
  ///
  /// \param _Graph The graph type the algorithm runs on. The default value
  /// is \ref ListGraph. The value of _Graph is not used directly by
  /// DagShortestPath, it is only passed to \ref DagShortestPathDefaultTraits.
  /// \param _LengthMap This read-only ArcMap determines the lengths of the
  /// arcs. The default map type is \ref concepts::Graph::ArcMap 
  /// "Graph::ArcMap<int>".  The value of _LengthMap is not used directly 
  /// by DagShortestPath, it is only passed to \ref DagShortestPathDefaultTraits.  
  /// \param _Traits Traits class to set various data types used by the 
  /// algorithm.  The default traits class is \ref DagShortestPathDefaultTraits
  /// "DagShortestPathDefaultTraits<_Graph,_LengthMap>".  See \ref
  /// DagShortestPathDefaultTraits for the documentation of a DagShortestPath traits
  /// class.
  ///
  /// \author Balazs Attila Mihaly (based on Balazs Dezso's work)

#ifdef DOXYGEN
  template <typename _Graph, typename _LengthMap, typename _Traits>
#else
  template <typename _Graph=ListGraph,
	    typename _LengthMap=typename _Graph::template ArcMap<int>,
	    typename _Traits=DagShortestPathDefaultTraits<_Graph,_LengthMap> >
#endif
  class DagShortestPath {
  public:
    
    /// \brief \ref Exception for uninitialized parameters.
    ///
    /// This error represents problems in the initialization
    /// of the parameters of the algorithms.
/*
    class UninitializedParameter : public lemon::UninitializedParameter {
    public:
      virtual const char* what() const throw() {
	return "lemon::DagShortestPath::UninitializedParameter";
      }
    };
*/
    typedef _Traits Traits;
    ///The type of the underlying graph.
    typedef typename _Traits::Graph Graph;

    typedef typename Graph::Node Node;
    typedef typename Graph::NodeIt NodeIt;
    typedef typename Graph::Arc Arc;
    typedef typename Graph::ArcIt ArcIt;
    typedef typename Graph::OutArcIt OutArcIt;
    
    /// \brief The type of the length of the arcs.
    typedef typename _Traits::LengthMap::Value Value;
    /// \brief The type of the map that stores the arc lengths.
    typedef typename _Traits::LengthMap LengthMap;
    /// \brief The type of the map that stores the last
    /// arcs of the shortest paths.
    typedef typename _Traits::PredMap PredMap;
    /// \brief The type of the map that stores the dists of the nodes.
    typedef typename _Traits::DistMap DistMap;
    /// \brief The operation traits.
    typedef typename _Traits::OperationTraits OperationTraits;
    /// \brief The Node weight map.
    typedef typename Graph::template NodeMap<Value> WeightMap;
  private:
    /// Pointer to the underlying graph
    const Graph *graph;
    /// Pointer to the length map
    const LengthMap *length;
    ///Pointer to the map of predecessors arcs
    PredMap *_pred;
    ///Indicates if \ref _pred is locally allocated (\c true) or not
    bool local_pred;
    ///Pointer to the map of distances
    DistMap *_dist;
    ///Indicates if \ref _dist is locally allocated (\c true) or not
    bool local_dist;
    ///Process step counter
    unsigned int _process_step;

    std::vector<Node> _node_order;

    /// Creates the maps if necessary.
    void create_maps() {
      if(!_pred) {
	local_pred = true;
	_pred = Traits::createPredMap(*graph);
      }
      if(!_dist) {
	local_dist = true;
	_dist = Traits::createDistMap(*graph);
      }
    }
    
  public :
 
    typedef DagShortestPath Create;

    /// \name Named template parameters

    ///@{

    template <class T>
    struct DefPredMapTraits : public Traits {
      typedef T PredMap;
      static PredMap *createPredMap(const Graph&) {
	//throw UninitializedParameter();
      }
    };

    /// \brief \ref named-templ-param "Named parameter" for setting PredMap 
    /// type
    /// \ref named-templ-param "Named parameter" for setting PredMap type
    ///
    template <class T>
    struct DefPredMap {
      typedef DagShortestPath< Graph, LengthMap, DefPredMapTraits<T> > Create;
    };
    
    template <class T>
    struct DefDistMapTraits : public Traits {
      typedef T DistMap;
      static DistMap *createDistMap(const Graph& graph) {
	//throw UninitializedParameter();
      }
    };

    /// \brief \ref named-templ-param "Named parameter" for setting DistMap 
    /// type
    ///
    /// \ref named-templ-param "Named parameter" for setting DistMap type
    ///
    template <class T>
    struct DefDistMap 
      : public DagShortestPath< Graph, LengthMap, DefDistMapTraits<T> > {
      typedef DagShortestPath< Graph, LengthMap, DefDistMapTraits<T> > Create;
    };
    
    template <class T>
    struct DefOperationTraitsTraits : public Traits {
      typedef T OperationTraits;
    };
    
    /// \brief \ref named-templ-param "Named parameter" for setting 
    /// OperationTraits type
    ///
    /// \ref named-templ-param "Named parameter" for setting OperationTraits
    /// type
    template <class T>
    struct DefOperationTraits
      : public DagShortestPath< Graph, LengthMap, DefOperationTraitsTraits<T> > {
      typedef DagShortestPath< Graph, LengthMap, DefOperationTraitsTraits<T> >
      Create;
    };
    
    ///@}

  protected:
    
    DagShortestPath() {}

  public:      
    
    /// \brief Constructor.
    ///
    /// \param _graph the graph the algorithm will run on.
    /// \param _length the length map used by the algorithm.
    DagShortestPath(const Graph& _graph, const LengthMap& _length) :
      graph(&_graph), length(&_length),
      _pred(0), local_pred(false),
      _dist(0), local_dist(false){}

    /// \brief Constructor with node weight map.
    ///
    /// \param _graph the graph the algorithm will run on.
    /// \param _length the length map used by the algorithm.
    /// The NodeMap _length will be used as the weight map.
    /// Each arc will have the weight of its target node.
    DagShortestPath(const Graph& _graph, const WeightMap& _length) :
      graph(&_graph),
      _pred(0), local_pred(false),
      _dist(0), local_dist(false){
      length=new LengthMap(_graph);
      _dist=new DistMap(_graph);
      for(ArcIt eit(_graph);eit!=INVALID;++eit)
	(const_cast<LengthMap*>(length))->set(eit,_length[_graph.target(eit)]);
      }

    ///Destructor.
    ~DagShortestPath() {
      if(local_pred) delete _pred;
      if(local_dist) delete _dist;
    }

    /// \brief Sets the length map.
    ///
    /// Sets the length map.
    /// \return \c (*this)
    DagShortestPath &lengthMap(const LengthMap &m) {
      length = &m;
      return *this;
    }

    /// \brief Sets the map storing the predecessor arcs.
    ///
    /// Sets the map storing the predecessor arcs.
    /// If you don't use this function before calling \ref run(),
    /// it will allocate one. The destuctor deallocates this
    /// automatically allocated map, of course.
    /// \return \c (*this)
    DagShortestPath &predMap(PredMap &m) {
      if(local_pred) {
	delete _pred;
	local_pred=false;
      }
      _pred = &m;
      return *this;
    }

    /// \brief Sets the map storing the distances calculated by the algorithm.
    ///
    /// Sets the map storing the distances calculated by the algorithm.
    /// If you don't use this function before calling \ref run(),
    /// it will allocate one. The destuctor deallocates this
    /// automatically allocated map, of course.
    /// \return \c (*this)
    DagShortestPath &distMap(DistMap &m) {
      if(local_dist) {
	delete _dist;
	local_dist=false;
      }
      _dist = &m;
      return *this;
    }

    /// \name Execution control
    /// The simplest way to execute the algorithm is to use
    /// one of the member functions called \c run(...)
    /// \n
    /// If you need more control on the execution,
    /// first you must call \ref init(...), then you can add several source
    /// nodes with \ref addSource().
    /// Finally \ref start() will perform the actual path computation.
    /// Some functions have an alternative form (\ref checkedInit(...),
    /// \ref checkedRun(...)) which also verifies if the graph given in the
    /// constructor is a dag.

    ///@{

    /// \brief Initializes the internal data structures.
    ///
    /// Initializes the internal data structures.
    void init(const Value value = OperationTraits::infinity()) {
      typedef typename Graph::template NodeMap<int> NodeOrderMap;
      _process_step=0;
      NodeOrderMap node_order(*graph);
      topologicalSort(*graph,node_order);
      _node_order.resize(countNodes(*graph),INVALID);
      create_maps();
      for (NodeIt it(*graph); it != INVALID; ++it) {
        _node_order[node_order[it]]=it;
        _pred->set(it, INVALID);
        _dist->set(it, value);
      }
    }

    /// \brief Initializes the internal data structures
    /// with a given topological sort (NodeMap).
    ///
    /// Initializes the internal data structures
    /// with a given topological sort (NodeMap).
    void init(const typename Graph::template NodeMap<int>& node_order,
         const Value value = OperationTraits::infinity()) {
      _process_step=0;
      _node_order.resize(countNodes(*graph),INVALID);
      create_maps();
      for (NodeIt it(*graph); it != INVALID; ++it) {
        _node_order[node_order[it]]=it;
        _pred->set(it, INVALID);
        _dist->set(it, value);
      }
    }

    /// \brief Initializes the internal data structures
    /// with a given topological sort (std::vector).
    ///
    /// Initializes the internal data structures
    /// with a given topological sort (std::vector).
    void init(const std::vector<Node>& node_order,
        const Value value = OperationTraits::infinity()) {
      _process_step=0;
      _node_order=node_order;
      create_maps();
      for (NodeIt it(*graph); it != INVALID; ++it) {
        _pred->set(it, INVALID);
        _dist->set(it, value);
      }
    }

    /// \brief Initializes the internal data structures. It also checks if the graph is dag.
    ///
    /// Initializes the internal data structures. It also checks if the graph is dag.
    /// \return true if the graph (given in the constructor) is dag, false otherwise.
    bool checkedInit(const Value value = OperationTraits::infinity()) {
      typedef typename Graph::template NodeMap<int> NodeOrderMap;
      NodeOrderMap node_order(*graph);
      if(!checkedTopologicalSort(*graph,node_order))return false;
      init(node_order,value);
      return true;
    }

    /// \brief Initializes the internal data structures with a given
    /// topological sort (NodeMap). It also checks if the graph is dag.
    ///
    /// Initializes the internal data structures with a given
    /// topological sort (NodeMap). It also checks if the graph is dag.
    /// \return true if the graph (given in the constructor) is dag, false otherwise.
    bool checkedInit(const typename Graph::template NodeMap<int>& node_order, 
                     const Value value = OperationTraits::infinity()) {
      for(NodeIt it(*graph);it!=INVALID;++it){
        for(OutArcIt oeit(*graph,it);oeit!=INVALID;++oeit){
          if(node_order[graph->target(oeit)]<node_order[it])return false;
        }
      }
      init(node_order,value);
      return true;
    }

    /// \brief Initializes the internal data structures with a given
    /// topological sort (std::vector). It also checks if the graph is dag.
    ///
    /// Initializes the internal data structures with a given
    /// topological sort (std::vector). It also checks if the graph is dag.
    /// \return true if the graph (given in the constructor) is dag, false otherwise.
    bool checkedInit(const std::vector<Node>& node_order, 
                     const Value value = OperationTraits::infinity()) {
      typedef typename Graph::template NodeMap<bool> BoolNodeMap;
      BoolNodeMap _processed(*graph,false);
      for(unsigned int i=0;i<_node_order.size();++i){
        _processed[node_order[i]]=true;
        for(OutArcIt oeit(*graph,node_order[i]);oeit!=INVALID;++oeit){
          if(_processed[graph->target(oeit)])return false;
        }
      }
      init(node_order,value);
      return true;
    }

    /// \brief Adds a new source node.
    ///
    /// The optional second parameter is the initial distance of the node.
    /// It just sets the distance of the node to the given value.
    void addSource(Node source, Value dst = OperationTraits::zero()) {
      if((*_dist)[source] != dst){
        _dist->set(source, dst);
      }
    }

    /// \brief Executes one step from the dag shortest path algorithm.
    ///
    /// If the algoritm calculated the distances in the previous step 
    /// strictly for all at most k length paths then it will calculate the 
    /// distances strictly for all at most k + 1 length paths. With k
    /// iteration this function calculates the at most k length paths.
    ///\pre the queue is not empty
    ///\return the currently processed node
    Node processNextNode() {
      if(reached(_node_order[_process_step])){
        for (OutArcIt it(*graph, _node_order[_process_step]); it != INVALID; ++it) {
	  Node target = graph->target(it);
	  Value relaxed =
	    OperationTraits::plus((*_dist)[_node_order[_process_step]], (*length)[it]);
	  if (OperationTraits::less(relaxed, (*_dist)[target])) {
	    _pred->set(target, it);
	    _dist->set(target, relaxed);
	  }
        }
      }
      ++_process_step;
      return _node_order[_process_step-1];
    }

    ///\brief Returns \c false if there are nodes
    ///to be processed in the queue
    ///
    ///Returns \c false if there are nodes
    ///to be processed in the queue
    bool emptyQueue() { return _node_order.size()-1==_process_step; }

    ///\brief Returns the number of the nodes to be processed.
    ///
    ///Returns the number of the nodes to be processed in the queue.
    int queueSize() { return _node_order.size()-1-_process_step; }

    /// \brief Executes the algorithm.
    ///
    /// \pre init() must be called and at least one node should be added
    /// with addSource() before using this function.
    ///
    /// This method runs the %DagShortestPath algorithm from the root node(s)
    /// in order to compute the shortest path to each node. The algorithm 
    /// computes 
    /// - The shortest path tree.
    /// - The distance of each node from the root(s).
    void start() {
      while(!emptyQueue()) {
	processNextNode();
      }
    }

    /// \brief Runs %DagShortestPath algorithm from node \c s.
    ///    
    /// This method runs the %DagShortestPath algorithm from a root node \c s
    /// in order to compute the shortest path to each node. The algorithm 
    /// computes
    /// - The shortest path tree.
    /// - The distance of each node from the root.
    ///
    /// \note d.run(s) is just a shortcut of the following code.
    ///\code
    ///  d.init();
    ///  d.addSource(s);
    ///  d.start();
    ///\endcode
    void run(Node s) {
      init();
      addSource(s);
      start();
    }
    
    /// \brief Runs %DagShortestPath algorithm from node \c s.
    /// It also checks if the graph is a dag.
    ///    
    /// This method runs the %DagShortestPath algorithm from a root node \c s
    /// in order to compute the shortest path to each node. The algorithm 
    /// computes
    /// - The shortest path tree.
    /// - The distance of each node from the root.
    /// The algorithm checks if the graph given int the constructor is a dag.
    bool checkedRun(Node s) {
      if(!checkedInit())return false;
      addSource(s);
      start();
      return true;
    }
    
    ///@}

    /// \name Query Functions
    /// The result of the %DagShortestPath algorithm can be obtained using these
    /// functions.\n
    /// Before the use of these functions,
    /// either run() or start() must be called.
    
    ///@{

    typedef PredMapPath<Graph, PredMap> Path;

    ///Gives back the shortest path.
    
    ///Gives back the shortest path.
    ///\pre The \c t should be reachable from the source.
    Path path(Node t) 
    {
      return Path(*graph, *_pred, t);
    }
	  
    /// \brief The distance of a node from the root.
    ///
    /// Returns the distance of a node from the root.
    /// \pre \ref run() must be called before using this function.
    /// \warning If node \c v in unreachable from the root the return value
    /// of this funcion is undefined.
    Value dist(Node v) const { return (*_dist)[v]; }

    /// \brief Returns the 'previous arc' of the shortest path tree.
    ///
    /// For a node \c v it returns the 'previous arc' of the shortest path 
    /// tree, i.e. it returns the last arc of a shortest path from the root 
    /// to \c v. It is \ref INVALID if \c v is unreachable from the root or 
    /// if \c v=s. The shortest path tree used here is equal to the shortest 
    /// path tree used in \ref predNode(). 
    /// \pre \ref run() must be called before using
    /// this function.
    Arc predArc(Node v) const { return (*_pred)[v]; }

    /// \brief Returns the 'previous node' of the shortest path tree.
    ///
    /// For a node \c v it returns the 'previous node' of the shortest path 
    /// tree, i.e. it returns the last but one node from a shortest path from 
    /// the root to \c /v. It is INVALID if \c v is unreachable from the root 
    /// or if \c v=s. The shortest path tree used here is equal to the 
    /// shortest path tree used in \ref predArc().  \pre \ref run() must be 
    /// called before using this function.
    Node predNode(Node v) const { 
      return (*_pred)[v] == INVALID ? INVALID : graph->source((*_pred)[v]); 
    }
    
    /// \brief Returns a reference to the NodeMap of distances.
    ///
    /// Returns a reference to the NodeMap of distances. \pre \ref run() must
    /// be called before using this function.
    const DistMap &distMap() const { return *_dist;}
 
    /// \brief Returns a reference to the shortest path tree map.
    ///
    /// Returns a reference to the NodeMap of the arcs of the
    /// shortest path tree.
    /// \pre \ref run() must be called before using this function.
    const PredMap &predMap() const { return *_pred; }
 
    /// \brief Checks if a node is reachable from the root.
    ///
    /// Returns \c true if \c v is reachable from the root.
    /// \pre \ref run() must be called before using this function.
    ///
    bool reached(Node v) { return (*_dist)[v] != OperationTraits::infinity(); }
    
    ///@}
  };
 
  /// \brief Default traits class of DagShortestPath function.
  ///
  /// Default traits class of DagShortestPath function.
  /// \param _Graph Graph type.
  /// \param _LengthMap Type of length map.
  template <typename _Graph, typename _LengthMap>
  struct DagShortestPathWizardDefaultTraits {
    /// \brief The graph type the algorithm runs on. 
    typedef _Graph Graph;

    /// \brief The type of the map that stores the arc lengths.
    ///
    /// The type of the map that stores the arc lengths.
    /// It must meet the \ref concepts::ReadMap "ReadMap" concept.
    typedef _LengthMap LengthMap;

    /// \brief The value type of the length map.
    typedef typename _LengthMap::Value Value;

    /// \brief Operation traits for dag shortest path algorithm.
    ///
    /// It defines the infinity type on the given Value type
    /// and the used operation.
    /// \see DagShortestPathDefaultOperationTraits
    typedef DagShortestPathDefaultOperationTraits<Value> OperationTraits;

    /// \brief The type of the map that stores the last
    /// arcs of the shortest paths.
    /// 
    /// The type of the map that stores the last
    /// arcs of the shortest paths.
    /// It must meet the \ref concepts::WriteMap "WriteMap" concept.
    typedef NullMap <typename _Graph::Node,typename _Graph::Arc> PredMap;

    /// \brief Instantiates a PredMap.
    /// 
    /// This function instantiates a \ref PredMap. 
    static PredMap *createPredMap(const _Graph &) {
      return new PredMap();
    }
    /// \brief The type of the map that stores the dists of the nodes.
    ///
    /// The type of the map that stores the dists of the nodes.
    /// It must meet the \ref concepts::WriteMap "WriteMap" concept.
    typedef NullMap<typename Graph::Node, Value> DistMap;
    /// \brief Instantiates a DistMap.
    ///
    /// This function instantiates a \ref DistMap. 
    static DistMap *createDistMap(const _Graph &) {
      return new DistMap();
    }
  };
  
  /// \brief Default traits used by \ref DagShortestPathWizard
  ///
  /// To make it easier to use DagShortestPath algorithm
  /// we have created a wizard class.
  /// This \ref DagShortestPathWizard class needs default traits,
  /// as well as the \ref DagShortestPath class.
  /// The \ref DagShortestPathWizardBase is a class to be the default traits of the
  /// \ref DagShortestPathWizard class.
  /// \todo More named parameters are required...
  template<class _Graph,class _LengthMap>
  class DagShortestPathWizardBase 
    : public DagShortestPathWizardDefaultTraits<_Graph,_LengthMap> {

    typedef DagShortestPathWizardDefaultTraits<_Graph,_LengthMap> Base;
  protected:
    /// Type of the nodes in the graph.
    typedef typename Base::Graph::Node Node;

    /// Pointer to the underlying graph.
    void *_graph;
    /// Pointer to the length map
    void *_length;
    ///Pointer to the map of predecessors arcs.
    void *_pred;
    ///Pointer to the map of distances.
    void *_dist;
    ///Pointer to the source node.
    Node _source;

    public:
    /// Constructor.
    
    /// This constructor does not require parameters, therefore it initiates
    /// all of the attributes to default values (0, INVALID).
    DagShortestPathWizardBase() : _graph(0), _length(0), _pred(0),
			   _dist(0), _source(INVALID) {}

    /// Constructor.
    
    /// This constructor requires some parameters,
    /// listed in the parameters list.
    /// Others are initiated to 0.
    /// \param graph is the initial value of  \ref _graph
    /// \param length is the initial value of  \ref _length
    /// \param source is the initial value of  \ref _source
    DagShortestPathWizardBase(const _Graph& graph, 
			  const _LengthMap& length, 
			  Node source = INVALID) :
      _graph((void *)&graph), _length((void *)&length), _pred(0),
      _dist(0), _source(source) {}

  };
  
  /// A class to make the usage of DagShortestPath algorithm easier

  /// This class is created to make it easier to use DagShortestPath algorithm.
  /// It uses the functions and features of the plain \ref DagShortestPath,
  /// but it is much simpler to use it.
  ///
  /// Simplicity means that the way to change the types defined
  /// in the traits class is based on functions that returns the new class
  /// and not on templatable built-in classes.
  /// When using the plain \ref DagShortestPath
  /// the new class with the modified type comes from
  /// the original class by using the ::
  /// operator. In the case of \ref DagShortestPathWizard only
  /// a function have to be called and it will
  /// return the needed class.
  ///
  /// It does not have own \ref run method. When its \ref run method is called
  /// it initiates a plain \ref DagShortestPath class, and calls the \ref 
  /// DagShortestPath::run() method of it.
  template<class _Traits>
  class DagShortestPathWizard : public _Traits {
    typedef _Traits Base;

    ///The type of the underlying graph.
    typedef typename _Traits::Graph Graph;

    typedef typename Graph::Node Node;
    typedef typename Graph::NodeIt NodeIt;
    typedef typename Graph::Arc Arc;
    typedef typename Graph::OutArcIt ArcIt;
    
    ///The type of the map that stores the arc lengths.
    typedef typename _Traits::LengthMap LengthMap;

    ///The type of the length of the arcs.
    typedef typename LengthMap::Value Value;

    ///\brief The type of the map that stores the last
    ///arcs of the shortest paths.
    typedef typename _Traits::PredMap PredMap;

    ///The type of the map that stores the dists of the nodes.
    typedef typename _Traits::DistMap DistMap;

  public:
    /// Constructor.
    DagShortestPathWizard() : _Traits() {}

    /// \brief Constructor that requires parameters.
    ///
    /// Constructor that requires parameters.
    /// These parameters will be the default values for the traits class.
    DagShortestPathWizard(const Graph& graph, const LengthMap& length, 
		      Node source = INVALID) 
      : _Traits(graph, length, source) {}

    /// \brief Copy constructor
    DagShortestPathWizard(const _Traits &b) : _Traits(b) {}

    ~DagShortestPathWizard() {}

    /// \brief Runs DagShortestPath algorithm from a given node.
    ///    
    /// Runs DagShortestPath algorithm from a given node.
    /// The node can be given by the \ref source function.
    void run() {
      // if(Base::_source == INVALID) throw UninitializedParameter();
      DagShortestPath<Graph,LengthMap,_Traits> 
	bf(*(Graph*)Base::_graph, *(LengthMap*)Base::_length);
      if (Base::_pred) bf.predMap(*(PredMap*)Base::_pred);
      if (Base::_dist) bf.distMap(*(DistMap*)Base::_dist);
      bf.run(Base::_source);
    }

    /// \brief Runs DagShortestPath algorithm from the given node.
    ///
    /// Runs DagShortestPath algorithm from the given node.
    /// \param source is the given source.
    void run(Node source) {
      Base::_source = source;
      run();
    }

    template<class T>
    struct DefPredMapBase : public Base {
      typedef T PredMap;
      static PredMap *createPredMap(const Graph &) { return 0; };
      DefPredMapBase(const _Traits &b) : _Traits(b) {}
    };
    
    ///\brief \ref named-templ-param "Named parameter"
    ///function for setting PredMap type
    ///
    /// \ref named-templ-param "Named parameter"
    ///function for setting PredMap type
    ///
    template<class T>
    DagShortestPathWizard<DefPredMapBase<T> > predMap(const T &t) 
    {
      Base::_pred=(void *)&t;
      return DagShortestPathWizard<DefPredMapBase<T> >(*this);
    }
    
    template<class T>
    struct DefDistMapBase : public Base {
      typedef T DistMap;
      static DistMap *createDistMap(const Graph &) { return 0; };
      DefDistMapBase(const _Traits &b) : _Traits(b) {}
    };
    
    ///\brief \ref named-templ-param "Named parameter"
    ///function for setting DistMap type
    ///
    /// \ref named-templ-param "Named parameter"
    ///function for setting DistMap type
    ///
    template<class T>
    DagShortestPathWizard<DefDistMapBase<T> > distMap(const T &t) {
      Base::_dist=(void *)&t;
      return DagShortestPathWizard<DefDistMapBase<T> >(*this);
    }

    template<class T>
    struct DefOperationTraitsBase : public Base {
      typedef T OperationTraits;
      DefOperationTraitsBase(const _Traits &b) : _Traits(b) {}
    };
    
    ///\brief \ref named-templ-param "Named parameter"
    ///function for setting OperationTraits type
    ///
    /// \ref named-templ-param "Named parameter"
    ///function for setting OperationTraits type
    ///
    template<class T>
    DagShortestPathWizard<DefOperationTraitsBase<T> > distMap() {
      return DagShortestPathWizard<DefDistMapBase<T> >(*this);
    }
    
    /// \brief Sets the source node, from which the DagShortestPath algorithm runs.
    ///
    /// Sets the source node, from which the DagShortestPath algorithm runs.
    /// \param source is the source node.
    DagShortestPathWizard<_Traits>& source(Node source) {
      Base::_source = source;
      return *this;
    }
    
  };
  
  /// \brief Function type interface for DagShortestPath algorithm.
  ///
  /// \ingroup flowalgs
  /// Function type interface for DagShortestPath algorithm.
  ///
  /// This function also has several \ref named-templ-func-param 
  /// "named parameters", they are declared as the members of class 
  /// \ref DagShortestPathWizard.
  /// The following
  /// example shows how to use these parameters.
  ///\code
  /// dagShortestPath(g,length,source).predMap(preds).run();
  ///\endcode
  /// \warning Don't forget to put the \ref DagShortestPathWizard::run() "run()"
  /// to the end of the parameter list.
  /// \sa DagShortestPathWizard
  /// \sa DagShortestPath
  template<class _Graph, class _LengthMap>
  DagShortestPathWizard<DagShortestPathWizardBase<_Graph,_LengthMap> >
  dagShortestPath(const _Graph& graph,
	      const _LengthMap& length, 
	      typename _Graph::Node source = INVALID) {
    return DagShortestPathWizard<DagShortestPathWizardBase<_Graph,_LengthMap> >
      (graph, length, source);
  }

} //END OF NAMESPACE LEMON

#endif

