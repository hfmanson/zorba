(:~
 : Module Name: eXcel-lookup
 : This module implements some Excel lookup functions. 
 : Module Version: 0.1
 : Date: August 1, 2009
 : Copyright:  2006-2008 The FLWOR Foundation. Licensed under the Apache License, Version 2.0.
 : Proprietary XQuery Extensions Used: None
 : XQuery Specification: January 2007
 : Module Overview: This is a library module offering the same set of functions
 : defined by Microsoft Excel into 
 : @see http://office.microsoft.com/en-us/excel/CH062528281033.aspx
 : @author  Daniel Turcanu
 :)
module namespace  excel = "http://www.zorba-xquery.com/modules/excel/lookup" ;

import module namespace excel-err="http://www.zorba-xquery.com/modules/excel/errors";
import module namespace excel-math="http://www.zorba-xquery.com/modules/excel/math";

(:~
  Uses index_num to return a value from the sequence of value arguments. 
  
  @see http://office.microsoft.com/en-us/excel/HP052090131033.aspx
  @param $index_num  the position in the sequence, 1 based
  @param $values the sequence of values
  @return the value at the index position
  @error XQP0021(errValue) if index is smaller than 1 or bigger than the size of sequence
  
:)
declare function excel:choose( $index_num as xs:integer, $values as xs:anyAtomicType*) as xs:anyAtomicType
{
  if($index_num < 1 or $index_num >  fn:count($values)) then
    fn:error($excel-err:errValue, "Choose function: index_num should be between 1 and value count ", $index_num)
  else
    $values[$index_num]
};

(:~
  Uses index_num to return a sequence from the list of sequences. 
  Use CHOOSE to select one of up to 29 sequences based on the index number.
   
  @see http://office.microsoft.com/en-us/excel/HP052090131033.aspx
  @param $index_num  the position in the sequence, 1 based
  @param $value_sequence1 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence2 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence3 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence4 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence5 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence6 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence7 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence8 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence9 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence10 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence11 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence12 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence13 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence14 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence15 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence16 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence17 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence18 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence19 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence20 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence21 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence22 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence23 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence24 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence25 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence26 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence27 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence28 a sequence of values. Specify the empty sequence () if you don't need it. 
  @param $value_sequence29 a sequence of values. Specify the empty sequence () if you don't need it. 
  @return the value at the index position
  @error XQP0021(errValue) if index is smaller than 1 or bigger than 29
:)
declare function excel:choose($index_num as xs:integer,
                              $value_sequence1 as xs:anyAtomicType*,
                              $value_sequence2 as xs:anyAtomicType*,
                              $value_sequence3 as xs:anyAtomicType*,
                              $value_sequence4 as xs:anyAtomicType*,
                              $value_sequence5 as xs:anyAtomicType*,
                              $value_sequence6 as xs:anyAtomicType*,
                              $value_sequence7 as xs:anyAtomicType*,
                              $value_sequence8 as xs:anyAtomicType*,
                              $value_sequence9 as xs:anyAtomicType*,
                              $value_sequence10 as xs:anyAtomicType*,
                              $value_sequence11 as xs:anyAtomicType*,
                              $value_sequence12 as xs:anyAtomicType*,
                              $value_sequence13 as xs:anyAtomicType*,
                              $value_sequence14 as xs:anyAtomicType*,
                              $value_sequence15 as xs:anyAtomicType*,
                              $value_sequence16 as xs:anyAtomicType*,
                              $value_sequence17 as xs:anyAtomicType*,
                              $value_sequence18 as xs:anyAtomicType*,
                              $value_sequence19 as xs:anyAtomicType*,
                              $value_sequence20 as xs:anyAtomicType*,
                              $value_sequence21 as xs:anyAtomicType*,
                              $value_sequence22 as xs:anyAtomicType*,
                              $value_sequence23 as xs:anyAtomicType*,
                              $value_sequence24 as xs:anyAtomicType*,
                              $value_sequence25 as xs:anyAtomicType*,
                              $value_sequence26 as xs:anyAtomicType*,
                              $value_sequence27 as xs:anyAtomicType*,
                              $value_sequence28 as xs:anyAtomicType*,
                              $value_sequence29 as xs:anyAtomicType*) as xs:anyAtomicType*
{
  if($index_num < 1 or $index_num >  29) then
    fn:error($excel-err:errValue, "Choose function: index_num should be between 1 and 29 ", $index_num)
  else
  if($index_num = 1) then
    $value_sequence1
  else
  if($index_num = 2) then
    $value_sequence2
  else
  if($index_num = 3) then
    $value_sequence3
  else
  if($index_num = 4) then
    $value_sequence4
  else
  if($index_num = 5) then
    $value_sequence5
  else
  if($index_num = 6) then
    $value_sequence6
  else
  if($index_num = 7) then
    $value_sequence7
  else
  if($index_num = 8) then
    $value_sequence8
  else
  if($index_num = 9) then
    $value_sequence9
  else
  if($index_num = 10) then
    $value_sequence10
  else
  if($index_num = 11) then
    $value_sequence11
  else
  if($index_num = 12) then
    $value_sequence12
  else
  if($index_num = 13) then
    $value_sequence13
  else
  if($index_num = 14) then
    $value_sequence14
  else
  if($index_num = 15) then
    $value_sequence15
  else
  if($index_num = 16) then
    $value_sequence16
  else
  if($index_num = 17) then
    $value_sequence17
  else
  if($index_num = 18) then
    $value_sequence18
  else
  if($index_num = 19) then
    $value_sequence19
  else
  if($index_num = 20) then
    $value_sequence20
  else
  if($index_num = 21) then
    $value_sequence21
  else
  if($index_num = 22) then
    $value_sequence22
  else
  if($index_num = 23) then
    $value_sequence23
  else
  if($index_num = 24) then
    $value_sequence24
  else
  if($index_num = 25) then
    $value_sequence25
  else
  if($index_num = 26) then
    $value_sequence26
  else
  if($index_num = 27) then
    $value_sequence27
  else
  if($index_num = 28) then
    $value_sequence28
  else
  if($index_num = 29) then
    $value_sequence29
  else
    fn:error($excel-err:errValue, "Choose function: index_num should be between 1 and 29 ", $index_num)
};
 
(:~
  Helper function for HLOOKUP, LOOKUP, MATCH, VLOOKUP.
  It searches a lookup_value in table_header with three different algorithms
        specified by range_lookup.
        
  @param $lookup_value the value to be searched. 
        Allowed types are numeric, string, boolean. 
        Boolean values are compared only with booleans.
        Numbers are compared only with numbers, if range_lookup is not zero.
        The other types are converted to string and compared to string value of all values.
  @param $table_header the sequence of values where to search
  @param $range_lookup specified the algorithm for searching.
        Can have values:
        1  = finds the largest value that is less than or equal to lookup_value.
                Table_header must be in ascending order.
        0  = finds the first value that is exactly equal to lookup_value. 
                Table_header can be in any order.
                If lookup_value is boolean, then only booleans are compared.
                For other types, they are casted to string and then compared using 
                xquery regular expressions. Lookup_value can be a xquery regular expression.
        -1 = finds the smallest value that is greater than or equal to lookup_value.
                Table_header must be in descending order.
   @param $pos the current position in original table_header
   @param $last_comparable_pos the position of last value that might be a match
   @return the value found
   @error XQP0021(errValue) if range_lookup=0 and the value cannot be found
        
:)
declare function excel:lookup_column(  $lookup_value as xs:anyAtomicType,
                                       $table_header as xs:anyAtomicType*,
                                       $range_lookup as xs:integer,
                                       $pos as xs:integer,
                                       $last_comparable_pos as xs:integer) as xs:integer
{
  if(fn:empty($table_header)) then
    if($range_lookup != 0) then
      $last_comparable_pos
    else
      fn:error($excel-err:errValue, "Hlookup function: cannot find lookup value ", $lookup_value)
  else
  if($range_lookup > 0) then
    if($lookup_value instance of xs:boolean) then
      if($table_header[1] instance of xs:boolean) then
        if($lookup_value = $table_header[1]) then
          $pos + 1
        else
        if($lookup_value < $table_header[1]) then
          $last_comparable_pos
        else
          excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $pos + 1)
      else
        excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $last_comparable_pos)
    else
    if(excel-math:is-a-number($lookup_value)) then
      if(excel-math:is-a-number($table_header[1])) then
        let $lookup_value_num := excel-math:cast-as-numeric($lookup_value)
        let $table_header_num := excel-math:cast-as-numeric($table_header[1]) return
        if($lookup_value_num = $table_header_num) then
          $pos + 1
        else if($lookup_value_num < $table_header_num) then
          $last_comparable_pos
        else
          excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $pos + 1)
      else
        excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $last_comparable_pos)
    else
    let $compare := fn:compare(fn:upper-case(fn:string($lookup_value)), 
                               fn:upper-case(fn:string($table_header[1]))) return
    if($compare = 0) then
      $pos + 1
    else if($compare < 0) then
      $last_comparable_pos
    else
      excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $pos + 1)
  else
  if($range_lookup < 0) then
    if($lookup_value instance of xs:boolean) then
      if($table_header[1] instance of xs:boolean) then
        if($lookup_value = $table_header[1]) then
          $pos + 1
        else
        if($lookup_value > $table_header[1]) then
          $last_comparable_pos
        else
          excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $pos + 1)
      else
        excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $last_comparable_pos)
    else
    if(excel-math:is-a-number($lookup_value)) then
      if(excel-math:is-a-number($table_header[1])) then
        let $lookup_value_num := excel-math:cast-as-numeric($lookup_value)
        let $table_header_num := excel-math:cast-as-numeric($table_header[1]) return
        if($lookup_value_num = $table_header_num) then
          $pos + 1
        else if($lookup_value_num > $table_header_num) then
          $last_comparable_pos
        else
          excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $pos + 1)
      else
        excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $last_comparable_pos)
    else
    let $compare := fn:compare(fn:upper-case(fn:string($lookup_value)), 
                               fn:upper-case(fn:string($table_header[1]))) return
    if($compare = 0) then
      $pos + 1
    else if($compare > 0) then
      $last_comparable_pos
    else
      excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, $pos + 1)
  else
    if($lookup_value instance of xs:boolean) then
      if($table_header[1] instance of xs:boolean) then
        if($lookup_value = $table_header[1]) then
          $pos + 1
        else
          excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, 0)
      else
        excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, 0)
    else
    if(fn:matches(fn:upper-case(fn:string($table_header[1])), 
                  fn:upper-case(fn:string($lookup_value)))) then
      $pos + 1
    else
      excel:lookup_column($lookup_value, fn:subsequence($table_header,2), $range_lookup, $pos + 1, 0)
        
};

(:~
  Searches for a value in the top row of an array of values, 
    and then returns a value in the same column from a row you specify in the array.  
  Array is specified with 3 parameters:
  table_array is a sequence of elements, first row first, then second row and so on
  table_width specifies the number of elements in a row
  table_height specifies the number of rows
  The number of elements in table_array must be equal or more than table_width * table_height
  
  For wildchar matching, the XQuery regex matcher is used.
  
  @see http://office.microsoft.com/en-us/excel/HP052091141033.aspx
  @param $lookup_value the value to be searched. 
        Allowed types are numeric, string, boolean. 
        Boolean values are compared only with booleans.
        Numbers are compared only with numbers, if range_lookup is not zero.
        The other types are converted to string and compared to string value of all values.
  @param $table_array the sequence of values, row after row
  @param $table_width the number of values in a row
  @param $table_height the number of rows
  @param $row_index_num the row index, 1 based
  @param $range_lookup specified the algorithm to use:
        true = find approximative match. 
               First row of array must be sorted in ascending order.
        false = find exact match, using xquery regex
               First row of array can be in any order.
  @return the value found, with original type
  @error XQP0021(errValue) if the array contains less elements than specified by table_height and table_width
  @error XQP0021(errValue) if row_index_num is outside the range 1 .. table_height
  @error XQP0021(errValue) if range_lookup is true and the value searched is smaller than
        the first value in the header
  @error XQP0021(errValue) if range_lookup=false and the value cannot be found
:)        
declare function excel:hlookup( $lookup_value as xs:anyAtomicType,
                                $table_array as xs:anyAtomicType+,
                                $table_width as xs:integer,
                                $table_height as xs:integer,
                                $row_index_num as xs:integer,
                                $range_lookup as xs:boolean) as xs:anyAtomicType
{
  if($table_height * $table_width > fn:count($table_array)) then
    fn:error($excel-err:errValue, "Hlookup function: table array width and height not specified correctly ", $table_width * 1000 + $table_height)
  else
  if($row_index_num < 1 or $row_index_num > $table_height) then
    fn:error($excel-err:errValue, "Hlookup function: row_index_num must be between 1 and table height ", $row_index_num)
  else
  let $table_header := (for $i in (1 to $table_width) return
                          $table_array[$i])
  let $column := excel:lookup_column($lookup_value, $table_header, $range_lookup cast as xs:integer, 0, 0) return
  if($column = 0) then
    fn:error($excel-err:errValue, "Hlookup function: lookup value is smaller than the first element in header", $lookup_value)
  else
  if($column > $table_width) then
    fn:error($excel-err:errValue, "Hlookup function: column found outside table array ", $column)
  else
    $table_array[($row_index_num - 1) * $table_width + $column]
};

(:~
  Same as above, only that range_lookup is defaulted to true.
  That is, this Hlookup looks for the approximate value 
    and the first row must be ordered ascending. 
:)
declare function excel:hlookup( $lookup_value as xs:anyAtomicType,
                                $table_array as xs:anyAtomicType+,
                                $table_width as xs:integer,
                                $table_height as xs:integer,
                                $row_index_num as xs:integer) as xs:anyAtomicType
{
  excel:hlookup($lookup_value, $table_array, $table_width, $table_height,
                $row_index_num, fn:true())
};

(:~
  Returns a value from within an array.
  This is the Array form of the Excel Index function.
  
  Array is specified with 3 parameters:
  array is a sequence of elements, first row first, then second row and so on
  array_height specifies the number of rows
  array_width specifies the number of elements in a row
  The number of elements in array must be equal or more than array_width * array_height
  
  @see http://office.microsoft.com/en-us/excel/HP052091381033.aspx
  @param $array the sequence of values, row after row
  @param $array_width the number of values in a row
  @param $array_height the number of rows
  @param $row_num the row position of the value, 1 based
  @param $column_num the column position of the value, 1 based
  @return the value from x-y in the array
  @error XQP0021(errValue) if the array contains less elements than specified by table_height and table_width
  @error XQP0021(errRef) if row_num is outside the range
:)
declare function excel:index($array as xs:anyAtomicType+,
                             $array_height as xs:integer,
                             $array_width as xs:integer,
                             $row_num as xs:integer,
                             $column_num as xs:integer) as xs:anyAtomicType+
{
  if($array_height * $array_width != fn:count($array)) then
    fn:error($excel-err:errValue, "Index function: array width and height not specified correctly ", $array_width * 1000 + $array_height)
  else
  if($row_num < 0 or $row_num > $array_height) then
    fn:error($excel-err:errRef, "Index function: row_num must be between 1 and array height or 0", $row_num)
  else
  if($column_num < 0 or $column_num > $array_width) then
    fn:error($excel-err:errRef, "Index function: column_num must be between 1 and array width or 0", $column_num)
  else
  if($array_height = 1) then
    if($column_num ge 1) then
      $array[$column_num]
    else
      fn:error($excel-err:errRef, "Index function: column_num must be between 1 and array width", $column_num)
  else
  if($array_width = 1) then
    if($row_num ge 1) then
      $array[$row_num]
    else
      fn:error($excel-err:errRef, "Index function: row_num must be between 1 and array width", $row_num)
  else
  if($row_num > 0 and $column_num > 0) then
    $array[($row_num - 1) * $array_width + $column_num]
  else
  if($column_num = 0) then
    for $i in (1 to $array_width) return
      $array[($row_num - 1)* $array_width + $i]
  else
    for $i in (1 to $array_height) return
      $array[($i - 1) * $array_width + $column_num]
};

(:~
  The Vector form
  Look in a sequence for a value 
    and return a value from the same position in a second sequence.
  If the value is not found, then it matches the largest value in lookup_vector 
    that is less than or equal to lookup_value.
  
  @see http://office.microsoft.com/en-us/excel/HP052091631033.aspx
  @param $lookup_value the value to be searched
  @param $lookup_vector the sequence to be searched, in ascending order.
  @param $result_vector the sequence containing the result values
  @return a value from $result_vector
  @error XQP0021(errNA) if lookup value is smaller than the first value in lookup_vector
  @error XQP0021(errNA) if position found is outside the result range
:)
declare function excel:lookup($lookup_value as xs:anyAtomicType,
                              $lookup_vector as xs:anyAtomicType+,
                              $result_vector as xs:anyAtomicType+) as xs:anyAtomicType
{
  let $pos := excel:lookup_column($lookup_value, $lookup_vector, 1, 0, 0) return
  if($pos = 0) then
    fn:error($excel-err:errNA, "Lookup function: lookup value is smaller than any vector values ", $lookup_value)
  else
  if($pos > fn:count($result_vector)) then    
    fn:error($excel-err:errNA, "Lookup function: lookup position is outside the result range ", $pos)
  else
    $result_vector[$pos]
};

(:~
  Array form
  It looks in the first row or column of an array for the specified value 
    and returns a value from the same position in the last row or column of the array.
  If array covers an area that is wider than it is tall (more columns than rows), 
    LOOKUP searches for lookup_value in the first row.
  If array is square or is taller than it is wide (more rows than columns), 
    LOOKUP searches in the first column.

  The values in the first row or first column must be in ascending order.
  
  @see http://office.microsoft.com/en-us/excel/HP052091631033.aspx
  @param $lookup_value the value to be searched.
    If the value is not found, then it matches the largest value in lookup_vector 
    that is less than or equal to lookup_value.
  @param $array the array sequence, row after row
  @param $array_width the number of values in a row
  @param $array_height the number of rows in the array
  @return the corresponding value in the last row or column
  @error XQP0021(errValue) if array contains less values than specified by array_width and array_height
            or array_width = 0 or array_height = 0
  @error XQP0021(errNA) if the lookup_value is smaller than the first value in the row or column
:)
declare function excel:lookup($lookup_value as xs:anyAtomicType,
                              $array as xs:anyAtomicType+,
                              $array_width as xs:integer,
                              $array_height as xs:integer) as xs:anyAtomicType
{
  if($array_height * $array_width != fn:count($array) or $array_height = 0 or $array_width = 0) then
    fn:error($excel-err:errValue, "Lookup function: array width and height not specified correctly ", $array_width * 1000 + $array_height)
  else
  if($array_width > $array_height) then
    let $header :=  (for $i in (1 to $array_width) return
                        $array[$i] )
    let $pos := excel:lookup_column($lookup_value, $header, 1, 0, 0) return
    if($pos = 0) then
      fn:error($excel-err:errNA, "Lookup function: lookup value is smaller than any vector values in the first row ", $lookup_value)
    else
      $array[($array_height - 1) * $array_width + $pos]
  else
    let $header :=  (for $i in (1 to $array_height) return
                      $array[($i - 1) * $array_width + 1] )
    let $pos := excel:lookup_column($lookup_value, $header, 1, 0, 0) return
    if($pos = 0) then
      fn:error($excel-err:errNA, "Lookup function: lookup value is smaller than any vector values in the first column ", $lookup_value)
    else
      $array[($pos - 1) * $array_width + $array_width]
 
};

(:~
  Returns the relative position of an item in a sequence that 
    matches a specified value in a specified order.
  Only for one dimensional vector.
  
  @see http://office.microsoft.com/en-us/excel/HP052091681033.aspx
  @param $lookup_value value to be searched.
  @param $sequence the vector where to search the value
  @param $match_type specifies the algorithm used for searching.
        Possible values:
        1  = finds the largest value that is less than or equal to lookup_value.
                Sequence must be in ascending order.
        0  = finds the first value that is exactly equal to lookup_value. 
                Sequence can be in any order.
                If lookup_value is boolean, then only booleans are compared.
                For other types, they are casted to string and then compared using 
                xquery regular expressions. Lookup_value can be a xquery regular expression.
        -1 = finds the smallest value that is greater than or equal to lookup_value.
                Sequence must be in descending order.
  @return the position of found value
  @error XQP0021(errNA) for match_type 1 or -1, the lookup_value is smaller or larger than
                the first value in sequence
  @error XQP0021(errValue) if range_lookup=0 and the value cannot be found
:)
declare function excel:match($lookup_value as xs:anyAtomicType,
                             $sequence as xs:anyAtomicType+,
                             $match_type as xs:integer) as xs:anyAtomicType
{
  let $pos := excel:lookup_column($lookup_value, $sequence, $match_type, 0, 0) return
  if($pos = 0) then
    fn:error($excel-err:errNA, "Match function: cannot match lookup value ", $lookup_value)
  else
    $pos  
};

(:~
  Same as above, but match_type is defaulted to 1.
  It finds the largest value that is less than or equal to lookup_value.
:)
declare function excel:match($lookup_value as xs:anyAtomicType,
                             $sequence as xs:anyAtomicType+) as xs:anyAtomicType
{
  excel:match($lookup_value, $sequence, 1)
};

(:~
  Returns a sub-array from an array.
  The inner array must be within the reference array
  
  @see http://office.microsoft.com/en-us/excel/HP052092081033.aspx
  @param $reference the reference array
  @param $reference_height the number of rows in the reference array
  @param $reference_width the number of elements in the reference array row
  @param $rows the relative row position where the sub-array starts.
      It must be a positive value, zero relative.
  @param $cols the relative column position where the sub-array starts.
      It must be a positive value, zero relative.
  @param $height the desired height of sub-array.
      The sub-array must be inside the reference array.
  @param $width the desired width of sub-array.
      The sub-array must be inside the reference array.
  @return the sequence specifying the sub-array, row after row
  @error XQP0021(errNA) rows or cols are negative
  @error XQP0021(errNA) height or width are smaller than 1
  @error XQP0021(errValue) reference array contains less elements than specified
         by reference_height and reference_width
  @error XQP0021(errNA) the resulted sub-array is not completely contained inside reference array
:)
declare function excel:offset($reference as xs:anyAtomicType+,
                              $reference_height as xs:integer,
                              $reference_width as xs:integer,
                              $rows as xs:integer,
                              $cols as xs:integer,
                              $height as xs:integer,
                              $width as xs:integer) as xs:anyAtomicType*
{
  if($rows < 0 or $cols < 0) then
    fn:error($excel-err:errNA, "Offset function: rows and cols must be positive or zero ", $rows * 1000 + $cols)
  else
  if($height < 1 or $width < 1) then
    fn:error($excel-err:errNA, "Offset function: height and width must be greater than zero", $height * 1000 + $width)
  else
  if(($reference_height * $reference_width) > fn:count($reference)) then
    fn:error($excel-err:errValue, "Offset function: reference width and height not specified correctly ", $reference_width * 1000 + $reference_height)
  else
  if(($rows + $height) > $reference_height or
     ($cols + $width) > $reference_width) then
    fn:error($excel-err:errNA, "Offset function: inner array is outside the reference array")
  else
    for $i in (1 to $height) return
      for $j in (1 to $width) return
        $reference[($rows + $i - 1) * $reference_width + $cols + $j]
};

(:~
  Same as above, only that the sub-array is specified only by rows and cols relative position.
  The sub-array height and width is computed to contain the remaining elements of the array.
:)
declare function excel:offset($reference as xs:anyAtomicType+,
                              $reference_height as xs:integer,
                              $reference_width as xs:integer,
                              $rows as xs:integer,
                              $cols as xs:integer) as xs:anyAtomicType*
{
  excel:offset($reference, $reference_height, $reference_width,
                $rows, $cols,
                $reference_height - $rows, $reference_width - $cols)
};

(:~
  Transposes an array. The rows become columns and vice versa.
  
  @see http://office.microsoft.com/en-us/excel/HP052093191033.aspx
  @param $array the sequence specifying the array, row after row
  @param $array_width the number of elements in a row
  @param $array_height the number of rows in the array
  @return the transposed array. It will be a sequence specifying an array, row after row.
        The result width is the input height.
        The result height is the input width.
  @error XQP0021(errValue) the array contains less elements than specified by array_width and array_height
:)
declare function excel:transpose($array as xs:anyAtomicType+,
                                 $array_width as xs:integer,
                                 $array_height as xs:integer) as xs:anyAtomicType+
{
  if(($array_height * $array_width) > fn:count($array)) then
    fn:error($excel-err:errValue, "Transpose function: array width and height not specified correctly ", $array_width * 1000 + $array_height)
  else
    for $c in (1 to $array_width) return
      for $r in (1 to $array_height) return
        $array[($r - 1) * $array_width + $c] 
};

(:~
  Searches for a value in the first column of a table array 
    and returns a value in the same row from another column in the table array.
  
  Array is specified with 3 parameters:
  table_array is a sequence of elements, first row first, then second row and so on
  table_width specifies the number of elements in a row
  table_height specifies the number of rows
  
  For wildchar matching, the XQuery regex matcher is used.
  
  @see http://office.microsoft.com/en-us/excel/HP052093351033.aspx
  @param $lookup_value the value to be searched. 
        Allowed types are numeric, string, boolean. 
        Boolean values are compared only with booleans.
        Numbers are compared only with numbers, if range_lookup is not zero.
        The other types are converted to string and compared to string value of all values.
  @param $table_array the sequence of values, row after row
  @param $table_width the number of values in a row
  @param $table_height the number of rows
  @param $col_index_num the row index, 1 based
  @param $range_lookup specified the algorithm to use:
        true = find approximative match. 
               First column of array must be sorted in ascending order.
        false = find exact match, using xquery regex
               First column of array can be in any order.
  @return the value found, with original type
  @error XQP0021(errValue) if the array contains less elements than specified by table_height and table_width
  @error XQP0021(errValue) if col_index_num is outside the range 1 .. table_height
  @error XQP0021(errValue) if range_lookup is true and the value searched is smaller than
        the first value in the first column
  @error XQP0021(errValue) if range_lookup=false and the value cannot be found
:)        
declare function excel:vlookup( $lookup_value as xs:anyAtomicType,
                                $table_array as xs:anyAtomicType+,
                                $table_width as xs:integer,
                                $table_height as xs:integer,
                                $col_index_num as xs:integer,
                                $range_lookup as xs:boolean) as xs:anyAtomicType
{
  if($table_height * $table_width > fn:count($table_array)) then
    fn:error($excel-err:errValue, "Vlookup function: table array width and height not specified correctly ", $table_width * 1000 + $table_height)
  else
  if($col_index_num < 1 or $col_index_num > $table_width) then
    fn:error($excel-err:errValue, "Vlookup function: col_index_num must be between 1 and table width ", $col_index_num)
  else
  let $table_header := (for $i in (1 to $table_height) return
                          $table_array[($i - 1) * $table_width + 1])
  let $row := excel:lookup_column($lookup_value, $table_header, $range_lookup cast as xs:integer, 0, 0) return
  if($row = 0) then
    fn:error($excel-err:errValue, "Vlookup function: lookup value cannot be matched", $lookup_value)
  else
  if($row > $table_height) then
    fn:error($excel-err:errValue, "Vlookup function: row found outside table array ", $row)
  else
    $table_array[($row - 1) * $table_width + $col_index_num]
};

(:~
  Same as above, with range_lookup defaulted to true.
  It finds the largest value that is less than or equal to lookup_value.
  First column must be in ascending order.
:)
declare function excel:vlookup( $lookup_value as xs:anyAtomicType,
                                $table_array as xs:anyAtomicType+,
                                $table_width as xs:integer,
                                $table_height as xs:integer,
                                $col_index_num as xs:integer) as xs:anyAtomicType
{
  excel:vlookup($lookup_value, $table_array, $table_width, $table_height,
                $col_index_num, fn:true())
};
