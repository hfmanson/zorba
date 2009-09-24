<xqdoc:xqdoc xmlns:xqdoc="http://www.xqdoc.org/1.0" xmlns:zorbadoc="http://www.zorba-xquery.com/zorba/doc">
<xqdoc:control>

<xqdoc:version>1.0</xqdoc:version>
</xqdoc:control>
<xqdoc:module type="library"><xqdoc:uri>http://www.28msec.com/modules/gdata</xqdoc:uri>
<xqdoc:name>gdata.xqlib</xqdoc:name>
<xqdoc:comment>
<xqdoc:description>

  Foo Bar module</xqdoc:description>
<xqdoc:see>http://example.com
</xqdoc:see>
</xqdoc:comment>
</xqdoc:module><xqdoc:imports><xqdoc:import><xqdoc:uri>http://www.zorba-xquery.com/zorba/internal-functions</xqdoc:uri></xqdoc:import>
<xqdoc:import><xqdoc:uri>http://www.28msec.com/modules/atom</xqdoc:uri></xqdoc:import>
<xqdoc:import><xqdoc:uri>http://www.zorba-xquery.com/zorba/rest-functions</xqdoc:uri><xqdoc:comment>
<xqdoc:description>

  Import the Atom XML Schema</xqdoc:description>
<xqdoc:see>http://www.kbcafe.com/rss/atom.xsd.xml
</xqdoc:see>
</xqdoc:comment>
</xqdoc:import>
</xqdoc:imports>
<xqdoc:variables><xqdoc:variable><xqdoc:uri>version</xqdoc:uri></xqdoc:variable>
<xqdoc:variable><xqdoc:uri>id</xqdoc:uri><xqdoc:comment>
<xqdoc:description>

  Google ID</xqdoc:description>
<xqdoc:see>http://www.google.com
</xqdoc:see>
</xqdoc:comment>
</xqdoc:variable>
<xqdoc:variable><xqdoc:uri>authToken</xqdoc:uri></xqdoc:variable>
</xqdoc:variables>
<xqdoc:functions><xqdoc:function><xqdoc:comment>
<xqdoc:description>

  Google Login</xqdoc:description>
<xqdoc:see>http://www.google.com
</xqdoc:see>
</xqdoc:comment>
<xqdoc:name>login</xqdoc:name>
<zorbadoc:read/><xqdoc:signature>declare function gdata:login($account as xs:string*, $password as xs:string+, $service as xs:string?) as xs:boolean+</xqdoc:signature>
<zorbadoc:parameters>
<zorbadoc:parameter name="$service" type="xs:string" occurrence="?"/>
<zorbadoc:parameter name="$password" type="xs:string" occurrence="+"/>
<zorbadoc:parameter name="$account" type="xs:string" occurrence="*"/>
</zorbadoc:parameters>
</xqdoc:function>
<xqdoc:function><xqdoc:name>get-headers</xqdoc:name>
<zorbadoc:read/><xqdoc:signature>declare function gdata:get-headers() as element(rest:headers)</xqdoc:signature>
<zorbadoc:parameters/>
</xqdoc:function>
<xqdoc:function><xqdoc:name>get-google-id</xqdoc:name>
<zorbadoc:read/><xqdoc:signature>declare function gdata:get-google-id() as xs:string</xqdoc:signature>
<zorbadoc:parameters/>
</xqdoc:function>
</xqdoc:functions>
</xqdoc:xqdoc>
