import module namespace json = "http://zorba.io/modules/json-xml";

let $json :=
  <json type="array">
    <foo type="boolean">true</foo>
  </json>
return json:serialize( $json )

(: vim:set et sw=2 ts=2: :)
