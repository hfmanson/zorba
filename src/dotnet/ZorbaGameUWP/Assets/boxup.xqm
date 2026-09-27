module namespace boxup="http://mansoft.nl/boxup";

declare function boxup:min-depth2($boxes, $column, $row) {
  for $box in $boxes[xs:integer(@column) eq $column][xs:integer(@row) eq $row]
  order by xs:integer($box/@depth)
  return $box
};

declare function boxup:min-depth($boxes, $column, $row) {
  boxup:min-depth2($boxes, $column, $row)[1]
};

declare updating function boxup:move($model-mover, $column, $row) {
  replace value of node $model-mover/@column with $column,
  replace value of node $model-mover/@row with $row  
};

declare updating function boxup:update($context, $model-mover, $x, $y, $new-x, $new-y, $dx, $dy) {
  let $boxes := $context/boxup:boxup/boxup:box
      , $mover-boxes := $boxes[@column eq $model-mover/@column][@row eq $model-mover/@row]
  return
  (
    boxup:move($model-mover, $new-x, $new-y),
    for $box in $mover-boxes
    let $box-depth1 := $mover-boxes[xs:integer(@depth) = 1]
    return
      if (xs:integer($box/@dx) ne -$dx or xs:integer($box/@dy) ne -$dy or xs:integer($box/@depth) eq 2 and not(empty($box-depth1)) and (xs:integer($box-depth1/@dx) ne -$dx or xs:integer($box-depth1/@dy) ne -$dy)) then
        (
          boxup:move($box, $new-x, $new-y)
        )
      else ()
    )
};

declare updating function boxup:check-move($context, $dx, $dy)  {
  let $model-element := $context/boxup:boxup
    , $model-mover := $model-element/boxup:mover
    , $boxes := $model-element/boxup:box
    , $blocks := $model-element/boxup:block
    , $column := xs:integer($model-mover/@column)
    , $row := xs:integer($model-mover/@row)
    , $new-column := $column + $dx
    , $new-row := $row + $dy
return
  (
    if ($new-column gt 0 and $new-column le xs:integer($model-element/@columns) and $new-row gt 0 and $new-row le xs:integer($model-element/@rows) and empty($blocks[xs:integer(@column) eq $new-column][xs:integer(@row) eq $new-row])) then
    (
      let $current-box := boxup:min-depth($boxes, $column, $row)
          , $new-box := boxup:min-depth($boxes, $new-column, $new-row)
      return
        if
          (
            empty($new-box) or
            xs:integer($new-box/@dx) eq $dx and xs:integer($new-box/@dy) eq $dy and (
              empty($current-box) or xs:integer($current-box/@depth) gt xs:integer($new-box/@depth) or xs:integer($current-box/@dx) eq -xs:integer($new-box/@dx) and xs:integer($current-box/@dy) eq -xs:integer($new-box/@dy)
            )
          )
        then
            boxup:update($context, $model-mover, $column, $row, $new-column, $new-row, $dx, $dy)
        else
          ()      
    )
    else
      ()
  )
};
