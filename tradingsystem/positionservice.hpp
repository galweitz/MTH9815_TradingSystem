/**
 * positionservice.hpp
 * Defines the data types and Service for positions.
 *
 * @author Breman Thuraisingham
 */
#ifndef POSITION_SERVICE_HPP
#define POSITION_SERVICE_HPP

#include <string>
#include <map>
#include "soa.hpp"
#include "tradebookingservice.hpp"

using namespace std;

/**
 * Position class in a particular book.
 * Type T is the product type.
 */
template<typename T>
class Position
{

public:

  // ctor for a position
  Position(const T &_product);

  // Get the product
  const T& GetProduct() const;

  // Get the position quantity
  long GetPosition(string &book);

  // Get the aggregate position
  long GetAggregatePosition();

  // Add position to a book
  void AddPosition(string& book, long quantity);
  
  std::string str() const;

private:
  T product;
  map<string,long> positions;

};

/**
 * Position Service to manage positions across multiple books and secruties.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class PositionService : public Service<string,Position <T> >
{

public:

  // Add a trade to the service
  virtual void AddTrade(const Trade<T> &trade) = 0;

};

template<typename T>
Position<T>::Position(const T &_product) :
  product(_product)
{
}

template<typename T>
const T& Position<T>::GetProduct() const
{
  return product;
}

template<typename T>
long Position<T>::GetPosition(string &book)
{
  return positions[book];
}

template<typename T>
long Position<T>::GetAggregatePosition()
{
  // No-op implementation - should be filled out for implementations
  return 0;
}

// add position to a book
template<typename T>
void Position<T>::AddPosition(string& book, long quantity)
{
    auto it = positions.find(book);
    if (it != positions.end())
    {
        it->second += quantity;
    }
    else
    {
        positions.insert(std::pair<string, long>(book, quantity));
    }
}

template <typename T>
std::string Position<T>::str() const
{
    std::stringstream ss;
    std::string id = product.GetProductId();
    ss << id << ",{";

    //position.GetAggregatePosition();
    for (const auto& x : positions)
    {
        ss << x.first << ":" << x.second << ",";
    }
    std::string s = ss.str();
    if (positions.size() > 0)
    {
        s = s.substr(0, s.size() - 1);
    }
    s += "}";
    return s;
}

#endif
