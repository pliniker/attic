

#include <map>
#include <vector>
#include <utility>
#include <cmath>

// don't include iostream if no dump facility
#ifndef NODUMP
#include <iostream>
#include <iomanip>
#endif



//
// This class enables the calculation of a correlation factor between x-axis
// and y-axis labels. The axis labels have no ordering significance.
// Correlation is defined as certain x and y values occurring frequently, and
// mutually exclusively of other y-values and x-values in the same row or
// column.
//
// This is implemented as an dimensionless sparse matrix.
//
struct correlation_t
{
    typedef uint16_t                            value_type;
    typedef uint8_t                             index_type;

    // private types
private:
    typedef std::map<index_type, value_type>    xaxis_t;
    typedef std::map<index_type, xaxis_t>       yaxis_t;

    typedef xaxis_t valid_entries;
    typedef valid_entries::const_iterator       valid_iter;

    typedef yaxis_t sparse_matrix;

    typedef xaxis_t::iterator                   xaxis_iter;
    typedef xaxis_t::const_iterator             xaxis_const_iter;

    typedef yaxis_t::iterator                   yaxis_iter;
    typedef yaxis_t::const_iterator             yaxis_const_iter;

    
    // public interface
public:
    // initialise to empty
    correlation_t() : m_total( 0 ) {}


    // update an input/output pair
    inline void increment( index_type x, index_type y )
    {
        yaxis_iter i = m_matrix.find( y );
        // no such row exists
        if( i == m_matrix.end() )
        {
            xaxis_t new_row;
            new_row.insert( std::make_pair( x, value_type(1) ) );
            m_matrix.insert( std::make_pair( y, new_row ) );
        }
        // the row already exists, lookup x
        else
        {
            xaxis_t& existing_row = i->second;

            xaxis_iter j = existing_row.find( x );
            // column entry doesn't exist
            if( j == existing_row.end() )
            {
                existing_row.insert( std::make_pair( x, value_type(1) ) );
            }
            else
            {
                j->second++;
            }
        }

        m_xaxis[ x ]++;
        m_yaxis[ y ]++;
        m_total++;
    }

    
    // set an input/output pair
    inline void set_entry( index_type x, index_type y, value_type count )
    {
        yaxis_iter i = m_matrix.find( y );
        // no such row exists
        if( i == m_matrix.end() )
        {
            xaxis_t new_row;
            new_row.insert( std::make_pair( x, value_type(count) ) );
            m_matrix.insert( std::make_pair( y, new_row ) );
        }
        // the row already exists, lookup x
        else
        {
            xaxis_t& existing_row = i->second;

            xaxis_iter j = existing_row.find( x );
            // column entry doesn't exist
            if( j == existing_row.end() )
            {
                existing_row.insert( std::make_pair( x, value_type(count) ) );
            }
            else
            {
                m_xaxis[ x ] -= j->second;
                m_yaxis[ y ] -= j->second;
                m_total -= j->second;

                j->second = count;
            }
        }

        m_xaxis[ x ] += count;
        m_yaxis[ y ] += count;
        m_total += count;
    }


    // return correlation factor
    inline double calculate()
    {
        double rv = 0.0;

        // only visit columns that have values
        valid_iter x = m_xaxis.begin();
        valid_iter xend = m_xaxis.end();
        while( x != xend )
        {
            index_type column = x->first;
            double stddev = std_dev_of_column( m_matrix, column );
            // weight column according to the number of entries in it versus
            // the sum total of all entries
            rv += ( stddev / (double)m_total * (double)x->second );
            x++;
        }

        // only visit rows that have values
        yaxis_const_iter y = m_matrix.begin();
        yaxis_const_iter yend = m_matrix.end();
        while( y != yend )
        {
            xaxis_t const& xs = y->second;
            double stddev = std_dev_of_row( xs, y->first );
            // weight row according to the number of entries in it versus the
            // sum total of all entries
            rv += ( stddev / (double)m_total * (double)m_yaxis[ y->first ] );
            y++;
        }

        return rv;
    }

    
    // reset matrix
    inline void reset()
    {
        m_matrix.clear();
        m_xaxis.clear();
        m_yaxis.clear();
        m_total = 0;
    }


#ifndef NODUMP
    // dump to stdout facility
    void dump()
    {
        std::cout << "           X  ";
        // print x-axis labels
        int count = 0;
        valid_iter x = m_xaxis.begin();
        valid_iter xend = m_xaxis.end();
        while( x != xend )
        {
            std::cout
                << std::setw( 10 )
                << std::setfill( ' ' )
                << (int)( x->first )
                << " ";
            count++;
            x++;
        }
        std::cout << std::endl;
        
        std::cout << "         Y +--";
        for( int k = 0; k < count; k++ )
        {
            std::cout << "-----------";
        }
        std::cout << std::endl;

        // print matrix with y-axis labels
        yaxis_const_iter y = m_matrix.begin();
        yaxis_const_iter yend = m_matrix.end();
        while( y != yend )
        {
            // print y-axis label
            std::cout 
                << std::setw( 10 ) 
                << std::setfill( ' ' ) 
                << (int)( y->first )
                << " | ";

            xaxis_t const& xs = y->second;

            valid_iter x = m_xaxis.begin();
            valid_iter xend = m_xaxis.end();
            while( x != xend )
            {
                xaxis_const_iter i = xs.find( x->first );
                if( i != xs.end() )
                {
                    std::cout 
                        << " "
                        << std::setw( 10 ) 
                        << std::setfill( ' ' ) 
                        << i->second;
                }
                else
                {
                    std::cout
                        << "          0";
                }

                x++;
            }
            
            std::cout << std::endl;

            y++;
        }

        std::cout << std::endl;
    }
#endif

    
    // private implementation
private:
    // return row value normalised by row total
    inline double norm_of_row_value( index_type row, value_type value )
    {
        return (double)value / (double)m_yaxis[ row ];
    }

    // return column value normalised by column total
    inline double norm_of_column_value( index_type column, value_type value )
    {
        return (double)value / (double)m_xaxis[ column ];
    }

    // calculate mean of row values
    inline double mean_of_row(          xaxis_t const&  xs, 
                                        index_type      row, 
                                        index_type      dim )
    {
        double rv = 0.0;

        xaxis_const_iter i = xs.begin();
        xaxis_const_iter iend = xs.end();
        while( i != iend )
        {
            rv += norm_of_row_value( row, i->second );
            i++;
        }

        rv /= dim;

#ifndef NODUMP
        std::cout 
            << "row mean    =" 
            << rv 
            << std::endl;
#endif

        return rv;
    }

    // calculate standard deviation of row values
    inline double std_dev_of_row( xaxis_t const& xs, index_type row )
    {
        double rv = 0.0;

        index_type xdim = m_xaxis.size();
        double mean = mean_of_row( xs, row, xdim );

        // count how many valid entries in the sparse matrix
        int count = 0;

        xaxis_const_iter i = xs.begin();
        xaxis_const_iter iend = xs.end();
        while( i != iend )
        {
            double xi = norm_of_row_value( row, i->second );
            rv += ( xi - mean ) * ( xi - mean );
            count++;
            i++;
        }

        // fill in zero'd columns
        rv += ( xdim - count ) * mean * mean;

        rv = std::sqrt( rv / xdim );

#ifndef NODUMP
        std::cout 
            << "row std_dev =" 
            << rv 
            << " count=" 
            << count 
            << " filled="
            << (xdim-count)
            << std::endl;
#endif

        return rv;
    }

    // calculate mean of column values
    inline double mean_of_column(       yaxis_t const&  ys, 
                                        index_type      column,
                                        index_type      dim )
    {
        double rv = 0.0;

        yaxis_const_iter i = ys.begin();
        yaxis_const_iter iend = ys.end();
        while( i != iend )
        {
            xaxis_t const& xs = i->second;

            xaxis_const_iter j = xs.find( column );
            if( j != xs.end() )
            {
                rv += norm_of_column_value( column, j->second );
            }

            i++;
        }

        rv /= dim;

#ifndef NODUMP
        std::cout 
            << "col mean    " 
            << column 
            << " =" 
            << rv 
            << std::endl;
#endif

        return rv;
    }

    // calculate standard deviation of column values
    inline double std_dev_of_column( yaxis_t const& ys, index_type column )
    {
        double rv = 0.0;
        
        index_type ydim = m_yaxis.size();
        double mean = mean_of_column( ys, column, ydim );

        // count how many valid entries in the sparse matrix
        int count = 0;

        yaxis_const_iter i = ys.begin();
        yaxis_const_iter iend = ys.end();
        while( i != iend )
        {
            xaxis_t const& xs = i->second;

            xaxis_const_iter j = xs.find( column );
            if( j != xs.end() )
            {
                double xi = norm_of_column_value( column, j->second );
                rv += ( xi - mean ) * ( xi - mean );
                count++;
            }

            i++;
        }

        // fill in zero'd columns
        rv += ( ydim - count ) * ( mean * mean );

        rv = std::sqrt( rv / ydim );

#ifndef NODUMP
        std::cout 
            << "col std_dev " 
            << column 
            << " =" 
            << rv 
            << " count=" 
            << count 
            << " filled="
            << (ydim-count)
            << std::endl;
#endif

        return rv;
    }


    // private data
private:
    sparse_matrix   m_matrix;
    valid_entries   m_xaxis;
    valid_entries   m_yaxis;
    value_type      m_total;
};

