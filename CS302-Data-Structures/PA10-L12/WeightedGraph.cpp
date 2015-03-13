/**
 * @file WeightedGraph.cpp
 * @author CatherinePollock
 * @date 11/19/14
 * 
 * This is the implementation file for the WeightedGraph.h file. 
*/

#include <stdexcept>
#include <iostream>
#include <climits>    /// For INT_MAX
#include <string>     /// Used for labels
#include "WeightedGraph.h"

using namespace std;

//--------------------------------------------------------------------

/// GRAPH PUBLIC FUNCTION IMPLEMENTATION -----------------------------

/**
 * default constructor
 *
 * Creates an empty graph. Allocates enough memory for a graph containing
 * maxNumber verticies. 
 * @param maxNumber int of max number of verticies
*/
WeightedGraph::WeightedGraph( int maxNumber )
{
   maxSize = maxNumber;
   size = 0;
   vertexList = new Vertex[ maxNumber ];
   adjMatrix = new int[ maxSize * maxSize ];
   pathMatrix = new int[ maxSize * maxSize ];
   clear();
}

/**
 * copy constructor
 *
 * Initializes the graph to be equivalent to the other
 * graph parameter.
 * @param other reference to a graph to be copied from
*/
WeightedGraph::WeightedGraph( const WeightedGraph& other )
{
   /// set this graph to equal other
   *this = other;
}

/**
 * overloaded assignment operator
 *
 * Sets the graph to be equivalent to the other graph 
 * parameter and returns a reference to this object.
 * @param other WeightedGraph reference to a heap to be copied from
 * @return WeightedGraph& reference to this graph
*/
WeightedGraph& WeightedGraph::operator= ( const WeightedGraph& other )
{
   int i, j;
   /// if this does not have the address of other
   if( this != &other )
   {
      /// clear this graph
      delete [] vertexList;
      delete [] adjMatrix;
      delete [] pathMatrix;
      size = other.size;
      maxSize = other.maxSize;
      vertexList = new Vertex[ maxSize ];
      adjMatrix = new int[ maxSize * maxSize ];
      pathMatrix = new int[ maxSize * maxSize ];
      
      /// copy data from other
      for( i = 0; i < size; i++ )
      {
         vertexList[ i ] = other.vertexList[ i ];
         for( j = 0; j < size; j++ )
         {
            setEdge( i, j, other.getEdge( i, j ) );
            setPath( i, j, other.getPath( i, j ) );
         }
      }
   }
   return *this;
}

/**
 * destructor
 *
 * Dellocates (frees) the memory used to store the graph. 
*/
WeightedGraph::~WeightedGraph()
{
   /// free memory for lists
   delete [] vertexList;
   delete [] adjMatrix;
   delete [] pathMatrix;
}

/**
 * insertVertex
 *
 * Inserts newVertex into graph. If the vertex already exists in graph, then
 * updates it. 
 * @param newVertex (const Vertex&) value given to insert. 
 * @exception logic_error thrown when graph is full
 * @pre graph is not full
*/
void WeightedGraph::insertVertex ( const Vertex& newVertex ) throw ( logic_error )
{
   int i, j;
   /// if graph is full, throw logic error
   if( isFull() )
   {
      throw logic_error( "Insert when graph is full" );
      return;
   }
   
   /// if existing vertex matches, update edges
   for( i = 0; i < size; i++ )
   {
      if( newVertex.getLabel() == vertexList[ i ].getLabel() )
      {
         vertexList[ i ] = newVertex;
         vertexList[ size ].setColor( newVertex.getColor() );
         return;
      }
   }
   
   /// otherwise, insert new vertex and incriment size
   vertexList[ size ] = newVertex;
   vertexList[ size ].setColor( newVertex.getColor() );
   size++;
}

/**
 * insertEdge
 *
 * Inserts an undirected edge connecting verticies v1 and v2 into the graph. 
 * The weight of the edge is wt. If there is already an edge connecting these verticies,
 * then updates the weight of the edge.  
 * @param wt (int) weight of edge 
 * @param v1 (const string&) first vertex to connect 
 * @param v2 (const string&) second vertex to connect
 * @pre graph contains v1 and v2
 * @exception logic_error thrown when graph does not contain v1 and/or v2
*/
void WeightedGraph::insertEdge ( const string& v1, const string& v2, int wt )
  throw ( logic_error )
{
   /// get indexes of verticies
   int v1index = getIndex( v1 );
   int v2index = getIndex( v2 );
   if( v1index == -1 || v2index == -1 )
   {
      throw logic_error( "One or more vertex not in graph" );
      return;
   }
   /// set the edge weight for verticies
   setEdge( v1index, v2index, wt );
   
   /// stuff needed to work i guess
   cout << "Size = " << size << ", idx_v1 = " << v1index << ", idx_v2 = " << v2index << endl;
}

/**
 * retrieveVertex
 *
 * Searches the graph for vertex v. If this is found, then places the value of the 
 * vertex's data in vData and returns true. Otherwise, returns false with vData undefined.  
 * @param vData (Vertex&) data found at found vertex
 * @param v (const string&) vertex to find
 * @return bool if vertex was found
*/
bool WeightedGraph::retrieveVertex ( const string& v, Vertex& vData ) const
{
   int i;
   
   /// if empty, return false
   if( isEmpty() )
   {
      return false;
   }
   
   /// iterate through vertexList
   for( i = 0; i < size; i++ )
   {
      /// if matching vertex, set vData and return true;
      if( v == vertexList[ i ].getLabel() )
      {
         vData = vertexList[ i ];
         return true;
      }
   }
   /// otherwise, return false
   return false;
}

/**
 * getEdgeWeight
 *
 * Searches the graph for the edge containing verticies v1 and v2. If this edge exists, 
 * then places the weight of the edge in wt and returns true. Otherwise, returns false
 * with wt undefined.
 * @param wt (int) weight of edge found 
 * @param v1 (const string&) first vertex to find
 * @param v2 (const string&) second vertex to find
 * @pre graph contains v1 and v2
 * @exception logic_error thrown when graph does not contain v1 and/or v2
 * @return bool if found edge with a weight
*/
bool WeightedGraph::getEdgeWeight ( const string& v1, const string& v2, int& wt ) const
  throw ( logic_error )
{
   /// get index of verticies
   int v1index = getIndex( v1 );
   int v2index = getIndex( v2 );
   if( v1index == -1 || v2index == -1 )
   {
      throw logic_error( "One or more vertex not in graph" );
   }
   /// get the weight of the edge
   wt = getEdge( v1index, v2index);
   
   /// return if weight found on edge
   return( wt != INFINITE_EDGE_WT );
}

/**
 * removeVertex
 *
 * Removes vertex from the graph and any edges connected to v. 
 * @param v (const string&) vertex to find and remove
 * @exception logic_error thrown if vertex v not in graph
 * @pre graph includes vertex v
*/
void WeightedGraph::removeVertex ( const string& v ) throw ( logic_error )
{
   int i, colIn, rowIn;
   
   for( i = 0; i < size; i++ )
   {
      if( v == vertexList[ i ].getLabel() )
      {
         /// shift following matrix cols left
         for( colIn = i; colIn < size; colIn++ )
         {
            for( rowIn = 0; rowIn < size; rowIn++ )
            {
               adjMatrix[ rowIn * maxSize + colIn ] = adjMatrix[ rowIn * maxSize + colIn + 1 ];
            }
         } 
         
         /// shift following matrix rows up
         for( colIn = 0; colIn < size; colIn++ )
         {
            for( rowIn = i; rowIn < size; rowIn++ )
            {
               adjMatrix[ rowIn * maxSize + colIn ] = adjMatrix[ ( rowIn + 1 ) * maxSize + colIn ];
            }
         } 
         
         /// shift verticies left
         for( colIn = i; colIn < size; colIn++ )
         {
            vertexList[ colIn ] = vertexList[ colIn + 1 ];
         }
         
         /// decrement size
         size--;
         return;
      }
   }
   throw logic_error( "Vertex not included in graph" );
}

/**
 * removeEdge
 *
 * Removes the edge connecting vertices v1 and v2 from the graph.
 * @param v1 (const string&) first vertex of edge 
 * @param v2 (const string&) second vertex of edge
 * @pre graph contains v1 and v2
 * @exception logic_error thrown when graph does not contain v1 and/or v2
*/
void WeightedGraph::removeEdge( const string& v1, const string& v2 ) throw ( logic_error )
{
   /// get index of verticies
   int v1index = getIndex( v1 );
   int v2index = getIndex( v2 );
   if( v1index == -1 || v2index == -1 )
   {
      throw logic_error( "One or more vertex not in graph" );
   }
   /// set edge to infinity
   setEdge( v1index, v2index, INFINITE_EDGE_WT );
}

/**
 * clear
 *
 * Removes all verticies and edges from graph.
*/
void WeightedGraph::clear()
{
   int i, j;
   Vertex temp;
   /// for all rows 
   for( i = 0; i < maxSize; i++ )
   {
      /// clear vertexList values
      vertexList[ i ] = temp;
      
      /// set all edges to infinity
      for( j = 0; j < maxSize; j++ )
      {
         setEdge( i, j, INFINITE_EDGE_WT );
      }
   }
   /// reset size
   size = 0;
}

/**
 * isEmpty
 *
 * Returns true if graph is empty (no verticies). Otherwise, returns false. 
 * @return bool if graph is empty
*/
bool WeightedGraph::isEmpty () const
{
   /// return if size is equal to zero
   return( size == 0 );
}

/**
 * isFull
 *
 * Returns true if graph is full (cannot add any more verticies). Otherwise,
 * returns false.
 * @return bool if graph is full
*/
bool WeightedGraph::isFull() const
{
   /// return if size is equal to maxSize
   return( size == maxSize );
}

/**
 * showStructure
 *
 * Outputs a graph's vertex list and adjacency matrix. This operation
 * is intended for testing/debugging purposes only.
*/
void WeightedGraph::showStructure () const
{
    if ( size == 0 ) {
       cout << "Empty graph" << endl;
    } else {
       cout << endl << "Vertex list : " << endl;
       for ( int row = 0 ; row < size ; row++ )
           cout << row << '\t' << vertexList[row].getLabel() << '\t' << vertexList[row].getColor() << endl;

       cout << endl << "Edge matrix : " << endl << '\t';
       for ( int col = 0 ; col < size ; col++ )
           cout << col << '\t';
       cout << endl;
       for ( int row = 0 ; row < size ; row++ )
       {
           cout << row << '\t';
           for ( int col = 0 ; col < size ; col++ )
           {
               int wt = getEdge(row,col);
               if ( wt == INFINITE_EDGE_WT )
                  cout << "- \t";
               else
                  cout << wt << '\t';
           }
           cout << endl;
       }
    }
}

/**
 * showShortestPaths
 *
 * Computes and displays the graph's path matrix.
*/
void WeightedGraph::showShortestPaths() const
{
   int i, j, k, wt;
   /// set values in path matrix
   for( i = 0; i < size; i++ )
   {
      for( j = 0; j < size; j++ )
      {
         /// set values in path matrix to those of edge matrix
         setPath( i, j, getEdge( i, j ) );
      }
      /// give main diagonal values 0 for path
      setPath( i, i, 0 );
   }
   
   /// loop through and set path values based on value
   for( i = 0; i < size; i++ )
   {
      for( j = 0; j < size; j++ )
      {
         for( k = 0; k < size; k++ )
         {
            /// if the path does not have infinity weight
            if( getPath( j, i ) != INFINITE_EDGE_WT && getPath( i, k )!= INFINITE_EDGE_WT )
            {
               /// if the path is less than previous calculated path
               if( getPath( j, i ) + getPath( i, k ) < getPath( j, k )
                   || getPath( j, k ) == INFINITE_EDGE_WT )
               {
                  /// set the path with the new value
                  setPath( j, k, ( getPath( j, i ) + getPath( i, k )) );
               }
            }
         }
      }
   }
   
   /// print path matrix
   cout << endl << "Path matrix: " << endl << '\t' ;
   for( i = 0; i < size; i++ )
   {
      cout << i << '\t';
   }
   cout << endl;
   
   /// print path value but '-' if infinity
   for( i = 0; i < size; i++ )
   {
      cout << i << '\t';
      for( j = 0; j < size; j++ )
      {
         wt = getPath( i, j );
      
         if( wt != INFINITE_EDGE_WT )
         {
            cout << wt << '\t';
         }
         else 
         {
            cout << '-' << '\t';
         }
      }
      cout << endl;
   }
}

/**
 * hasProperColoring
 *
 * Returns true if no vertex in graph has same color as adjacent vertex.
 * Otherwise, returns false. 
 * @return bool if no vertex has same color as adjacent vertex
 * @pre all verticies have been assigned a color.
*/
bool WeightedGraph::hasProperColoring() const
{
   int i, j;
   /// loop through all values
   for( i = 0; i < size; i++ )
   {
      for( j = 0; j < size; j++ )
      {
         /// if the vertex has a value
         if( getEdge( i, j ) != INFINITE_EDGE_WT )
         {
            /// if the vetex has same value, return false 
            if( i != j && vertexList[ i ].getColor() == vertexList[ j ].getColor() )
            {
               return false;
            }
         }
      }
   }
   /// otherwise return true
   return true;
}

/**
 * areAllEven
 *
 * Returns true if every vertex in a graph is of even degree.
 * Otherwise, returns false. 
 * @return bool if all verticies are of even degree
*/
bool WeightedGraph::areAllEven() const
{
   int i, j, k = 0;
   /// loop throught all values in graph
   for( i = 0; i < size; i++ )
   {
      for( j = 0; j < size; j++ )
      {
         /// if the edge has a value, incriment k
         if( getEdge( i, j ) != INFINITE_EDGE_WT )
         {
            k++;
         }
      }
      /// if there are an odd number of valued edges, return false
      if( k % 2 != 0 )
      {
         return false;
      }  
      k = 0;
   }
   return true;
}

/// GRAPH FACILITATOR FUNCTIONS --------------------------------------------

/**
 * getIndex
 *
 * Returns the adjacency matrix index for vertex v. Returns size if
 * the vertex does not exist. 
 * @return int of index
 * @param v (char*) character pointer to vertex label
*/
int WeightedGraph::getIndex( const string& v ) const
{
   /// search for passed string and return index
   int i;
   for( i = 0; i < size; i++ )
   {
      if( vertexList[ i ].getLabel() == v )
      {
         return i;
      }
   }
   return -1;
}

/**
 * getEdge
 *
 * Get edge weight using adjacency matrix indicies.
 * @return int of weight
 * @param row (int) index of row
 * @param col (int) index of column
*/
int WeightedGraph::getEdge( int row, int col ) const
{
   /// return edge value at index
   return adjMatrix[ row * maxSize + col ];
}

/**
 * setEdge
 *
 * Set edge weight using adjacency matrix indicies.
 * @param wt (int) weight value
 * @param row (int) index of row
 * @param col (int) index of column
*/
void WeightedGraph::setEdge ( int row, int col, int wt)
{
   /// set edge values for specific row and column to wt
   adjMatrix[ row * maxSize + col ] = wt;
   adjMatrix[ col * maxSize + row ] = wt;
}

/**
 * getPath
 *
 * Get edge path using adjacency matrix indicies.
 * @return int of weight
 * @param row (int) index of row
 * @param col (int) index of column
*/
int WeightedGraph::getPath( int row, int col ) const
{
   /// return edge value at index
   return pathMatrix[ (row * maxSize) + col ];
}

/**
 * setPath
 *
 * Set edge path using adjacency matrix indicies.
 * @param wt (int) weight value
 * @param row (int) index of row
 * @param col (int) index of column
*/
void WeightedGraph::setPath ( int row, int col, int wt) const
{
   /// set path values for specific row and column to wt
   pathMatrix[ (row * maxSize) + col ] = wt;
   pathMatrix[ (col * maxSize) + row ] = wt;
}



