#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {
  
  class buffer ;

  /**
   *  @brief  api class.
   *  Helper class to perform high level operation on sio objects
   */
  class api {
  public:
    // static API only
    api() = delete ;

    /**
     *  @name Buffer I/O
     */
    ///@{
    /**
     *  @brief  Read data from the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to read from
     *  @param  ptr the address of the variable to receive
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to read
     *  @return the actual number of bytes read out
     */
    template <class bufT, typename T>
    static typename bufT::size_type read( const bufT &buffer, T *ptr, typename bufT::index_type position, typename bufT::size_type count ) ;

    /**
     *  @brief  Read data from the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to read from
     *  @param  ptr the address of the variable to receive
     *  @param  length the length of the variable
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to read
     *  @return the actual number of bytes read out
     */
    template <class bufT>
    static typename bufT::size_type read( const bufT &buffer, typename bufT::pointer ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) ;

    /**
     *  @brief  Write data to the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to write to
     *  @param  ptr the address of the variable to write
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to write
     *  @return the actual number of bytes written out
     */
    template <class bufT, typename T>
    static typename bufT::size_type write( bufT &buffer, const T *const ptr, typename bufT::index_type position, typename bufT::size_type count ) ;

    /**
     *  @brief  Write data to the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to write to
     *  @param  ptr the address of the variable to write
     *  @param  length the length of the variable
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to write
     *  @return the actual number of bytes written out
     */
    template <class bufT>
    static typename bufT::size_type write( bufT &buffer, typename bufT::const_pointer const ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) ;
    ///@}

    /**
     *  @name Record I/O
     */
    ///@{
    /**
     *  @brief  Read the next record header from the input stream.
     *          On exit, the stream cursor is set after the record header, ready
     *          to read the incoming record data bytes. The record header bytes 
     *          read out from the stream are stored in the buffer passed as third 
     *          argument. If the buffer is not large enough to contain all bytes,
     *          it is expanded.  
     *  
     *  @param  stream the input stream
     *  @param  rec_info the record info to receive
     *  @param  outbuf the buffer containing the record info bytes
     */
    static void read_record_info( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) ;
    
    /**
     *  @brief  Read out the record data from the input stream. The record data 
     *          bytes are written in the buffer passed by reference. By default, the 
     *          bytes are written at the beginning of the buffer. The last argument
     *          allows for specifying a shift from the start of the buffer. 
     *          
     *  @param  stream the input stream
     *  @param  rec_info the record info
     *  @param  outbuf the buffer to receive the record data bytes
     *  @param  buffer_shift an optional shift from the start of the buffer
     */
    static void read_record_data( sio::ifstream &stream, const record_info &rec_info, buffer &outbuf, std::size_t buffer_shift = 0 ) ;
    
    /**
     *  @brief  Read out the record (header + data) from the input stream.
     *          Simple combination of the functions above.
     *   
     *  @param  stream the input stream
     *  @param  rec_info the record info to receive
     *  @param  outbuf the record header + data bytes to receive
     */
    static void read_record( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) ;
    
    /**
     *  @brief  Read out the record (header + data) from the input stream.
     *          Simple combination of the functions above. Returns the record
     *          info and the buffer. The initial buffer size is set to 1 Mo and
     *          might be expanded while reading out the record data if required.
     *          
     *  @param  stream the input stream
     */
    static std::pair<record_info, buffer> read_record( sio::ifstream &stream ) ;
    
    /**
     *  @brief  Skip the next records while the unary predicate is true.
     *          The predicate must be of the following form :
     *          'bool predicate(const record_info&)'.
     *          
     *  @param  stream the input stream
     *  @param  pred the unary predicate
     */
    template <class UnaryPredicate>
    static void skip_records( sio::ifstream &stream, UnaryPredicate pred ) ;
    
    /**
     *  @brief  Skip the N next records from the input stream
     *  
     *  @param  stream the input stream
     *  @param  nskip the number of record to skip
     */
    static void skip_records( sio::ifstream &stream, std::size_t nskip ) ;
    
    /**
     *  @brief  Skip the N next records with a specific name.
     *          If a record with a different name is encountered, it is also skipped.
     *           
     *  @param  stream the input stream
     *  @param  nskip the number of record to skip
     *  @param  name the record name to skip (only)
     */
    static void skip_records( sio::ifstream &stream, std::size_t nskip, const std::string &name ) ;
    
    /**
     *  @brief  Got to the next record with the spcified name
     *  
     *  @param  stream the input stream
     *  @param  name the target record name
     */
    static void go_to_record( sio::ifstream &stream, const std::string &name ) ;
    
    /**
     *  @brief  Extract all the block info from the buffer. Skip block reading
     *  
     *  @param  buf the input block buffer
     */
    static std::vector<block_info> read_block_infos( const buffer_span &buf ) ;
    
    /**
     *  @brief  Extract the block info and get a buffer span of the block at the given index
     *  
     *  @param  rec_buf the record buffer
     *  @param  index the index of block header start in the record buffer
     */
    static std::pair<block_info, buffer_span> extract_block( const buffer_span &rec_buf, buffer_span::index_type index ) ;
    
    /**
     *  @brief  Dump the records from the input stream to the console.
     *          Note that if you use a detailed printout, the record 
     *          is first uncompressed using zlib and the block infos 
     *          are printed out too.
     *  
     *  @param  stream the input stream
     *  @param  skip the number of records to skip from the current position
     *  @param  count the number of record to printout
     *  @param  detailed whether to printout detailed information (block info)
     */
    static void dump_records( sio::ifstream &stream, std::size_t skip, std::size_t count, bool detailed ) ;
    ///@}

    /**
     *  @name Compression
     */
    ///@{
    /**
     *  @brief  Extract the compression bit from the option word
     * 
     *  @param  opts the options word
     */
    static bool is_compressed( options_type opts ) ;
    
    /**
     *  @brief  Uncompress the buffer and return a new buffer
     *
     *  @param  compressor the compression implementation
     *  @param  inbuf the input buffer to uncompress
     *  @param  outbuf the output buffer to receive
     *  @param  args the uncompression options (implementation dependant)
     */
    template <typename compT, typename ...Args>
    static void uncompress( compT &compressor, const buffer_span &inbuf, buffer &outbuf, Args ...args ) ;
    
    /**
     *  @brief  Compress the buffer and return a new buffer
     *
     *  @param  compressor the compression implementation
     *  @param  inbuf the input buffer to compress
     *  @param  outbuf the output buffer to receive
     *  @param  args the compression options (implementation dependant)
     */
    template <typename compT, typename ...Args>
    static void compress( compT &compressor, const buffer_span &inbuf, buffer &outbuf, Args ...args ) ;
    ///@}
  };
  
}

#include <sio/details/api_impl.h>