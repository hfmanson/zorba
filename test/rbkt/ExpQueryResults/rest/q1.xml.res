<?xml version="1.0" encoding="UTF-8"?>
<zorba-rest:payload xmlns:zorba-rest="http://www.zorba-xquery.com/zorba/rest-functions"><html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=us-ascii" />
    <title>HTML Tidy Configuration Options Quick Reference</title>
    <link type="text/css" rel="stylesheet" href="tidy.css" />
  </head>
  <body>
    <h1 id="top">Quick Reference</h1>
    <h2>HTML Tidy Configuration Options</h2>
    <p>
      <em>Generated automatically with HTML Tidy released
    on 18 June 2008.</em>
    </p>
    <p>
      <a class="h3" href="#MarkupHeader">HTML, XHTML, XML</a>
      <br />
      <a class="h3" href="#DiagnosticsHeader">Diagnostics</a>
      <br />
      <a class="h3" href="#PrettyPrintHeader">Pretty Print</a>
      <br />
      <a class="h3" href="#EncodingHeader">Character Encoding</a>
      <br />
      <a class="h3" href="#MiscellaneousHeader">Miscellaneous</a>
    </p>
    <table summary="Tidy Options Quick Reference Header Section" border="0" cellpadding="3" cellspacing="0">
      <colgroup>
        <col width="33%" />
        <col width="33%" />
        <col width="33%" />
      </colgroup>
      <tr valign="bottom">
        <td class="h3" colspan="2" id="MarkupHeader">HTML, XHTML, XML Options</td>
        <td valign="top">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td class="tabletitle">Option</td>
        <td class="tabletitle">Type</td>
        <td class="tabletitle">Default</td>
      </tr>
      <tr>
        <td>
          <a href="#add-xml-decl">add-xml-decl</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#add-xml-space">add-xml-space</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#alt-text">alt-text</a>
        </td>
        <td>String</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#anchor-as-name">anchor-as-name</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#assume-xml-procins">assume-xml-procins</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#bare">bare</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#clean">clean</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#css-prefix">css-prefix</a>
        </td>
        <td>String</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#decorate-inferred-ul">decorate-inferred-ul</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#doctype">doctype</a>
        </td>
        <td>DocType</td>
        <td>auto</td>
      </tr>
      <tr>
        <td>
          <a href="#drop-empty-paras">drop-empty-paras</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#drop-font-tags">drop-font-tags</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#drop-proprietary-attributes">drop-proprietary-attributes</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#enclose-block-text">enclose-block-text</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#enclose-text">enclose-text</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#escape-cdata">escape-cdata</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#fix-backslash">fix-backslash</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#fix-bad-comments">fix-bad-comments</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#fix-uri">fix-uri</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#hide-comments">hide-comments</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#hide-endtags">hide-endtags</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#indent-cdata">indent-cdata</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#input-xml">input-xml</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#join-classes">join-classes</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#join-styles">join-styles</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#literal-attributes">literal-attributes</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#logical-emphasis">logical-emphasis</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#lower-literals">lower-literals</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#merge-divs">merge-divs</a>
        </td>
        <td>AutoBool</td>
        <td>auto</td>
      </tr>
      <tr>
        <td>
          <a href="#merge-spans">merge-spans</a>
        </td>
        <td>AutoBool</td>
        <td>auto</td>
      </tr>
      <tr>
        <td>
          <a href="#ncr">ncr</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#new-blocklevel-tags">new-blocklevel-tags</a>
        </td>
        <td>Tag names</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#new-empty-tags">new-empty-tags</a>
        </td>
        <td>Tag names</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#new-inline-tags">new-inline-tags</a>
        </td>
        <td>Tag names</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#new-pre-tags">new-pre-tags</a>
        </td>
        <td>Tag names</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#numeric-entities">numeric-entities</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#output-html">output-html</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#output-xhtml">output-xhtml</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#output-xml">output-xml</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#preserve-entities">preserve-entities</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#quote-ampersand">quote-ampersand</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#quote-marks">quote-marks</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#quote-nbsp">quote-nbsp</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#repeated-attributes">repeated-attributes</a>
        </td>
        <td>enum</td>
        <td>keep-last</td>
      </tr>
      <tr>
        <td>
          <a href="#replace-color">replace-color</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#show-body-only">show-body-only</a>
        </td>
        <td>AutoBool</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#uppercase-attributes">uppercase-attributes</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#uppercase-tags">uppercase-tags</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#word-2000">word-2000</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr valign="bottom">
        <td colspan="3"> </td>
      </tr>
      <tr valign="bottom">
        <td class="h3" colspan="2" id="DiagnosticsHeader">Diagnostics Options</td>
        <td valign="top">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td class="tabletitle">Option</td>
        <td class="tabletitle">Type</td>
        <td class="tabletitle">Default</td>
      </tr>
      <tr>
        <td>
          <a href="#accessibility-check">accessibility-check</a>
        </td>
        <td>enum</td>
        <td>0 (Tidy Classic)</td>
      </tr>
      <tr>
        <td>
          <a href="#show-errors">show-errors</a>
        </td>
        <td>Integer</td>
        <td>6</td>
      </tr>
      <tr>
        <td>
          <a href="#show-warnings">show-warnings</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr valign="bottom">
        <td colspan="3"> </td>
      </tr>
      <tr valign="bottom">
        <td class="h3" colspan="2" id="PrettyPrintHeader">Pretty Print Options</td>
        <td valign="top">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td class="tabletitle">Option</td>
        <td class="tabletitle">Type</td>
        <td class="tabletitle">Default</td>
      </tr>
      <tr>
        <td>
          <a href="#break-before-br">break-before-br</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#indent">indent</a>
        </td>
        <td>AutoBool</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#indent-attributes">indent-attributes</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#indent-spaces">indent-spaces</a>
        </td>
        <td>Integer</td>
        <td>2</td>
      </tr>
      <tr>
        <td>
          <a href="#markup">markup</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#punctuation-wrap">punctuation-wrap</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#sort-attributes">sort-attributes</a>
        </td>
        <td>enum</td>
        <td>none</td>
      </tr>
      <tr>
        <td>
          <a href="#split">split</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#tab-size">tab-size</a>
        </td>
        <td>Integer</td>
        <td>8</td>
      </tr>
      <tr>
        <td>
          <a href="#vertical-space">vertical-space</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#wrap">wrap</a>
        </td>
        <td>Integer</td>
        <td>68</td>
      </tr>
      <tr>
        <td>
          <a href="#wrap-asp">wrap-asp</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#wrap-attributes">wrap-attributes</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#wrap-jste">wrap-jste</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#wrap-php">wrap-php</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#wrap-script-literals">wrap-script-literals</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#wrap-sections">wrap-sections</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr valign="bottom">
        <td colspan="3"> </td>
      </tr>
      <tr valign="bottom">
        <td class="h3" colspan="2" id="EncodingHeader">Character Encoding Options</td>
        <td valign="top">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td class="tabletitle">Option</td>
        <td class="tabletitle">Type</td>
        <td class="tabletitle">Default</td>
      </tr>
      <tr>
        <td>
          <a href="#ascii-chars">ascii-chars</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#char-encoding">char-encoding</a>
        </td>
        <td>Encoding</td>
        <td>ascii</td>
      </tr>
      <tr>
        <td>
          <a href="#input-encoding">input-encoding</a>
        </td>
        <td>Encoding</td>
        <td>latin1</td>
      </tr>
      <tr>
        <td>
          <a href="#language">language</a>
        </td>
        <td>String</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#newline">newline</a>
        </td>
        <td>enum</td>
        <td>
          <em>Platform dependent</em>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#output-bom">output-bom</a>
        </td>
        <td>AutoBool</td>
        <td>auto</td>
      </tr>
      <tr>
        <td>
          <a href="#output-encoding">output-encoding</a>
        </td>
        <td>Encoding</td>
        <td>ascii</td>
      </tr>
      <tr valign="bottom">
        <td colspan="3"> </td>
      </tr>
      <tr valign="bottom">
        <td class="h3" colspan="2" id="MiscellaneousHeader">Miscellaneous Options</td>
        <td valign="top">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td class="tabletitle">Option</td>
        <td class="tabletitle">Type</td>
        <td class="tabletitle">Default</td>
      </tr>
      <tr>
        <td>
          <a href="#error-file">error-file</a>
        </td>
        <td>String</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#force-output">force-output</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#gnu-emacs">gnu-emacs</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#gnu-emacs-file">gnu-emacs-file</a>
        </td>
        <td>String</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#keep-time">keep-time</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#output-file">output-file</a>
        </td>
        <td>String</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#quiet">quiet</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr>
        <td>
          <a href="#slide-style">slide-style</a>
        </td>
        <td>String</td>
        <td>
          <strong>-</strong>
        </td>
      </tr>
      <tr>
        <td>
          <a href="#tidy-mark">tidy-mark</a>
        </td>
        <td>Boolean</td>
        <td>yes</td>
      </tr>
      <tr>
        <td>
          <a href="#write-back">write-back</a>
        </td>
        <td>Boolean</td>
        <td>no</td>
      </tr>
      <tr valign="bottom">
        <td colspan="3"> </td>
      </tr>
    </table>
    <table summary="Tidy Options Quick Reference Detail Section" border="0" cellpadding="3" cellspacing="0">
      <tr>
        <td> </td>
      </tr>
      <tr valign="bottom">
        <td valign="top" colspan="2" class="h2" id="MarkupReference">HTML, XHTML, XML Options Reference
    </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="add-xml-decl">add-xml-decl</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#char-encoding">char-encoding</a>
          <br />
          <a href="#output-encoding">output-encoding</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should add the XML declaration when outputting XML or XHTML. Note that if the input already includes an &lt;?xml ... ?&gt; declaration then this option will be ignored. If the encoding for the output is different from "ascii", one of the utf encodings or "raw", the declaration is always added as required by the XML standard. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="add-xml-space">add-xml-space</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should add xml:space="preserve" to elements such as &lt;PRE&gt;, &lt;STYLE&gt; and &lt;SCRIPT&gt; when generating XML. This is needed if the whitespace in such elements is to be parsed appropriately without having access to the DTD. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="alt-text">alt-text</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>String</strong><br />
            Default: <strong>-</strong><br />Example: <strong>-</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies the default "alt=" text Tidy uses for &lt;IMG&gt; attributes. This feature is dangerous as it suppresses further accessibility warnings. You are responsible for making your documents accessible to people who can not see the images! </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="anchor-as-name">anchor-as-name</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option controls the deletion or addition of the name attribute in elements where it can serve as anchor. If set to "yes", a name attribute, if not already existing, is added along an existing id attribute if the DTD allows it. If set to "no", any existing name attribute is removed if an id attribute exists or has been added. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="assume-xml-procins">assume-xml-procins</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should change the parsing of processing instructions to require ?&gt; as the terminator rather than &gt;. This option is automatically set if the input is in XML. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="bare">bare</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should strip Microsoft specific HTML from Word 2000 documents, and output spaces rather than non-breaking spaces where they exist in the input. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="clean">clean</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#drop-font-tags">drop-font-tags</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should strip out surplus presentational tags and attributes replacing them by style rules and structural markup as appropriate. It works well on the HTML saved by Microsoft Office products. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="css-prefix">css-prefix</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>String</strong><br />
            Default: <strong>-</strong><br />Example: <strong>-</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies the prefix that Tidy uses for styles rules. By default, "c" will be used. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="decorate-inferred-ul">decorate-inferred-ul</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should decorate inferred UL elements with some CSS markup to avoid indentation to the right. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="doctype">doctype</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>DocType</strong><br />
            Default: <strong>auto</strong><br />Example: <strong>omit, auto, strict, transitional, user</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies the DOCTYPE declaration generated by Tidy. If set to "omit" the output won't contain a DOCTYPE declaration. If set to "auto" (the default) Tidy will use an educated guess based upon the contents of the document. If set to "strict", Tidy will set the DOCTYPE to the strict DTD. If set to "loose", the DOCTYPE is set to the loose (transitional) DTD. Alternatively, you can supply a string for the formal public identifier (FPI).<br /><br />For example: <br />doctype: "-//ACME//DTD HTML 3.14159//EN"<br /><br />If you specify the FPI for an XHTML document, Tidy will set the system identifier to an empty string. For an HTML document, Tidy adds a system identifier only if one was already present in order to preserve the processing mode of some browsers. Tidy leaves the DOCTYPE for generic XML documents unchanged. <code>--doctype omit</code> implies <code>--numeric-entities yes</code>. This option does not offer a validation of the document conformance. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="drop-empty-paras">drop-empty-paras</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should discard empty paragraphs. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="drop-font-tags">drop-font-tags</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#clean">clean</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should discard &lt;FONT&gt; and &lt;CENTER&gt; tags without creating the corresponding style rules. This option can be set independently of the clean option. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="drop-proprietary-attributes">drop-proprietary-attributes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should strip out proprietary attributes, such as MS data binding attributes. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="enclose-block-text">enclose-block-text</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should insert a &lt;P&gt; element to enclose any text it finds in any element that allows mixed content for HTML transitional but not HTML strict. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="enclose-text">enclose-text</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should enclose any text it finds in the body element within a &lt;P&gt; element. This is useful when you want to take existing HTML and use it with a style sheet. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="escape-cdata">escape-cdata</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should convert &lt;![CDATA[]]&gt; sections to normal text. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="fix-backslash">fix-backslash</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should replace backslash characters "<code>\</code>" in URLs by forward slashes "<code>/</code>". </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="fix-bad-comments">fix-bad-comments</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should replace unexpected hyphens with "=" characters when it comes across adjacent hyphens. The default is yes. This option is provided for users of Cold Fusion which uses the comment syntax: &lt;!--- ---&gt; </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="fix-uri">fix-uri</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should check attribute values that carry URIs for illegal characters and if such are found, escape them as HTML 4 recommends. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="hide-comments">hide-comments</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should print out comments. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="hide-endtags">hide-endtags</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should omit optional end-tags when generating the pretty printed markup. This option is ignored if you are outputting to XML. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="indent-cdata">indent-cdata</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should indent &lt;![CDATA[]]&gt; sections. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="input-xml">input-xml</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should use the XML parser rather than the error correcting HTML parser. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="join-classes">join-classes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#join-styles">join-styles</a>
          <br />
          <a href="#repeated-attributes">repeated-attributes</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should combine class names to generate a single new class name, if multiple class assignments are detected on an element. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="join-styles">join-styles</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#join-classes">join-classes</a>
          <br />
          <a href="#repeated-attributes">repeated-attributes</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should combine styles to generate a single new style, if multiple style values are detected on an element. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="literal-attributes">literal-attributes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should ensure that whitespace characters within attribute values are passed through unchanged. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="logical-emphasis">logical-emphasis</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should replace any occurrence of &lt;I&gt; by &lt;EM&gt; and any occurrence of &lt;B&gt; by &lt;STRONG&gt;. In both cases, the attributes are preserved unchanged. This option can be set independently of the clean and drop-font-tags options. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="lower-literals">lower-literals</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should convert the value of an attribute that takes a list of predefined values to lower case. This is required for XHTML documents. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="merge-divs">merge-divs</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>AutoBool</strong><br />
            Default: <strong>auto</strong><br />Example: <strong>auto, y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#clean">clean</a>
          <br />
          <a href="#merge-spans">merge-spans</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">Can be used to modify behavior of -c (--clean yes) option. This option specifies if Tidy should merge nested &lt;div&gt; such as "&lt;div&gt;&lt;div&gt;...&lt;/div&gt;&lt;/div&gt;". If set to "auto", the attributes of the inner &lt;div&gt; are moved to the outer one. As well, nested &lt;div&gt; with ID attributes are not merged. If set to "yes", the attributes of the inner &lt;div&gt; are discarded with the exception of "class" and "style". </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="merge-spans">merge-spans</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>AutoBool</strong><br />
            Default: <strong>auto</strong><br />Example: <strong>auto, y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#clean">clean</a>
          <br />
          <a href="#merge-divs">merge-divs</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">Can be used to modify behavior of -c (--clean yes) option. This option specifies if Tidy should merge nested &lt;span&gt; such as "&lt;span&gt;&lt;span&gt;...&lt;/span&gt;&lt;/span&gt;". The algorithm is identical to the one used by --merge-divs. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="ncr">ncr</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should allow numeric character references. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="new-blocklevel-tags">new-blocklevel-tags</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Tag names</strong><br />
            Default: <strong>-</strong><br />Example: <strong>tagX, tagY, ...</strong></td>
        <td align="right" valign="top">
          <a href="#new-empty-tags">new-empty-tags</a>
          <br />
          <a href="#new-inline-tags">new-inline-tags</a>
          <br />
          <a href="#new-pre-tags">new-pre-tags</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies new block-level tags. This option takes a space or comma separated list of tag names. Unless you declare new tags, Tidy will refuse to generate a tidied file if the input includes previously unknown tags. Note you can't change the content model for elements such as &lt;TABLE&gt;, &lt;UL&gt;, &lt;OL&gt; and &lt;DL&gt;. This option is ignored in XML mode. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="new-empty-tags">new-empty-tags</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Tag names</strong><br />
            Default: <strong>-</strong><br />Example: <strong>tagX, tagY, ...</strong></td>
        <td align="right" valign="top">
          <a href="#new-blocklevel-tags">new-blocklevel-tags</a>
          <br />
          <a href="#new-inline-tags">new-inline-tags</a>
          <br />
          <a href="#new-pre-tags">new-pre-tags</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies new empty inline tags. This option takes a space or comma separated list of tag names. Unless you declare new tags, Tidy will refuse to generate a tidied file if the input includes previously unknown tags. Remember to also declare empty tags as either inline or blocklevel. This option is ignored in XML mode. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="new-inline-tags">new-inline-tags</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Tag names</strong><br />
            Default: <strong>-</strong><br />Example: <strong>tagX, tagY, ...</strong></td>
        <td align="right" valign="top">
          <a href="#new-blocklevel-tags">new-blocklevel-tags</a>
          <br />
          <a href="#new-empty-tags">new-empty-tags</a>
          <br />
          <a href="#new-pre-tags">new-pre-tags</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies new non-empty inline tags. This option takes a space or comma separated list of tag names. Unless you declare new tags, Tidy will refuse to generate a tidied file if the input includes previously unknown tags. This option is ignored in XML mode. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="new-pre-tags">new-pre-tags</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Tag names</strong><br />
            Default: <strong>-</strong><br />Example: <strong>tagX, tagY, ...</strong></td>
        <td align="right" valign="top">
          <a href="#new-blocklevel-tags">new-blocklevel-tags</a>
          <br />
          <a href="#new-empty-tags">new-empty-tags</a>
          <br />
          <a href="#new-inline-tags">new-inline-tags</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies new tags that are to be processed in exactly the same way as HTML's &lt;PRE&gt; element. This option takes a space or comma separated list of tag names. Unless you declare new tags, Tidy will refuse to generate a tidied file if the input includes previously unknown tags. Note you can not as yet add new CDATA elements (similar to &lt;SCRIPT&gt;). This option is ignored in XML mode. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="numeric-entities">numeric-entities</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#doctype">doctype</a>
          <br />
          <a href="#preserve-entities">preserve-entities</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output entities other than the built-in HTML entities (&amp;amp;, &amp;lt;, &amp;gt; and &amp;quot;) in the numeric rather than the named entity form. Only entities compatible with the DOCTYPE declaration generated are used. Entities that can be represented in the output encoding are translated correspondingly. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="output-html">output-html</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should generate pretty printed output, writing it as HTML. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="output-xhtml">output-xhtml</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should generate pretty printed output, writing it as extensible HTML. This option causes Tidy to set the DOCTYPE and default namespace as appropriate to XHTML. If a DOCTYPE or namespace is given they will checked for consistency with the content of the document. In the case of an inconsistency, the corrected values will appear in the output. For XHTML, entities can be written as named or numeric entities according to the setting of the "numeric-entities" option. The original case of tags and attributes will be preserved, regardless of other options. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="output-xml">output-xml</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should pretty print output, writing it as well-formed XML. Any entities not defined in XML 1.0 will be written as numeric entities to allow them to be parsed by a XML parser. The original case of tags and attributes will be preserved, regardless of other options. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="preserve-entities">preserve-entities</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should preserve the well-formed entitites as found in the input. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="quote-ampersand">quote-ampersand</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output unadorned &amp; characters as &amp;amp;. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="quote-marks">quote-marks</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output " characters as &amp;quot; as is preferred by some editing environments. The apostrophe character ' is written out as &#39; since many web browsers don't yet support &amp;apos;. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="quote-nbsp">quote-nbsp</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output non-breaking space characters as entities, rather than as the Unicode character value 160 (decimal). </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="repeated-attributes">repeated-attributes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>enum</strong><br />
            Default: <strong>keep-last</strong><br />Example: <strong>keep-first, keep-last</strong></td>
        <td align="right" valign="top">
          <a href="#join-classes">join-classes</a>
          <br />
          <a href="#join-styles">join-styles</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should keep the first or last attribute, if an attribute is repeated, e.g. has two align attributes. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="replace-color">replace-color</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should replace numeric values in color attributes by HTML/XHTML color names where defined, e.g. replace "#ffffff" with "white". </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="show-body-only">show-body-only</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>AutoBool</strong><br />
            Default: <strong>no</strong><br />Example: <strong>auto, y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should print only the contents of the body tag as an HTML fragment. If set to "auto", this is performed only if the body tag has been inferred. Useful for incorporating existing whole pages as a portion of another page. This option has no effect if XML output is requested. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="uppercase-attributes">uppercase-attributes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output attribute names in upper case. The default is no, which results in lower case attribute names, except for XML input, where the original case is preserved. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="uppercase-tags">uppercase-tags</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output tag names in upper case. The default is no, which results in lower case tag names, except for XML input, where the original case is preserved. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="word-2000">word-2000</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should go to great pains to strip out all the surplus stuff Microsoft Word 2000 inserts when you save Word documents as "Web pages". Doesn't handle embedded images or VML. You should consider using Word's "Save As: Web Page, Filtered". </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr valign="bottom">
        <td valign="top" colspan="2" class="h2" id="DiagnosticsReference">Diagnostics Options Reference
    </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="accessibility-check">accessibility-check</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>enum</strong><br />
            Default: <strong>0 (Tidy Classic)</strong><br />Example: <strong>0 (Tidy Classic), 1 (Priority 1 Checks), 2 (Priority 2 Checks), 3 (Priority 3 Checks)</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies what level of accessibility checking, if any, that Tidy should do. Level 0 is equivalent to Tidy Classic's accessibility checking. For more information on Tidy's accessibility checking, visit the <a href="http://www.aprompt.ca/Tidy/accessibilitychecks.html">Adaptive Technology Resource Centre at the University of Toronto</a>. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="show-errors">show-errors</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Integer</strong><br />
            Default: <strong>6</strong><br />Example: <strong>0, 1, 2, ...</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies the number Tidy uses to determine if further errors should be shown. If set to 0, then no errors are shown. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="show-warnings">show-warnings</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should suppress warnings. This can be useful when a few errors are hidden in a flurry of warnings. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr valign="bottom">
        <td valign="top" colspan="2" class="h2" id="PrettyPrintReference">Pretty Print Options Reference
    </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="break-before-br">break-before-br</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output a line break before each &lt;BR&gt; element. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="indent">indent</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>AutoBool</strong><br />
            Default: <strong>no</strong><br />Example: <strong>auto, y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#indent-spaces">indent-spaces</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should indent block-level tags. If set to "auto", this option causes Tidy to decide whether or not to indent the content of tags such as TITLE, H1-H6, LI, TD, TD, or P depending on whether or not the content includes a block-level element. You are advised to avoid setting indent to yes as this can expose layout bugs in some browsers. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="indent-attributes">indent-attributes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should begin each attribute on a new line. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="indent-spaces">indent-spaces</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Integer</strong><br />
            Default: <strong>2</strong><br />Example: <strong>0, 1, 2, ...</strong></td>
        <td align="right" valign="top">
          <a href="#indent">indent</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies the number of spaces Tidy uses to indent content, when indentation is enabled. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="markup">markup</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should generate a pretty printed version of the markup. Note that Tidy won't generate a pretty printed version if it finds significant errors (see force-output). </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="punctuation-wrap">punctuation-wrap</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should line wrap after some Unicode or Chinese punctuation characters. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="sort-attributes">sort-attributes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>enum</strong><br />
            Default: <strong>none</strong><br />Example: <strong>none, alpha</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies that tidy should sort attributes within an element using the specified sort algorithm. If set to "alpha", the algorithm is an ascending alphabetic sort. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="split">split</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">Currently not used. Tidy Classic only. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="tab-size">tab-size</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Integer</strong><br />
            Default: <strong>8</strong><br />Example: <strong>0, 1, 2, ...</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies the number of columns that Tidy uses between successive tab stops. It is used to map tabs to spaces when reading the input. Tidy never outputs tabs. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="vertical-space">vertical-space</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should add some empty lines for readability. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="wrap">wrap</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Integer</strong><br />
            Default: <strong>68</strong><br />Example: <strong>0 (no wrapping), 1, 2, ...</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies the right margin Tidy uses for line wrapping. Tidy tries to wrap lines so that they do not exceed this length. Set wrap to zero if you want to disable line wrapping. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="wrap-asp">wrap-asp</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should line wrap text contained within ASP pseudo elements, which look like: &lt;% ... %&gt;. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="wrap-attributes">wrap-attributes</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#wrap-script-literals">wrap-script-literals</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should line wrap attribute values, for easier editing. This option can be set independently of wrap-script-literals. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="wrap-jste">wrap-jste</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should line wrap text contained within JSTE pseudo elements, which look like: &lt;# ... #&gt;. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="wrap-php">wrap-php</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should line wrap text contained within PHP pseudo elements, which look like: &lt;?php ... ?&gt;. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="wrap-script-literals">wrap-script-literals</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#wrap-attributes">wrap-attributes</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should line wrap string literals that appear in script attributes. Tidy wraps long script string literals by inserting a backslash character before the line break. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="wrap-sections">wrap-sections</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should line wrap text contained within &lt;![ ... ]&gt; section tags. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr valign="bottom">
        <td valign="top" colspan="2" class="h2" id="EncodingReference">Character Encoding Options Reference
    </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="ascii-chars">ascii-chars</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top">
          <a href="#clean">clean</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">Can be used to modify behavior of -c (--clean yes) option.  If set to "yes" when using -c, &amp;emdash;, &amp;rdquo;, and other named character entities are downgraded to their closest ascii equivalents. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="char-encoding">char-encoding</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Encoding</strong><br />
            Default: <strong>ascii</strong><br />Example: <strong>raw, ascii, latin0, latin1, utf8, iso2022, mac, win1252, ibm858, utf16le, utf16be, utf16, big5, shiftjis</strong></td>
        <td align="right" valign="top">
          <a href="#input-encoding">input-encoding</a>
          <br />
          <a href="#output-encoding">output-encoding</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies the character encoding Tidy uses for both the input and output. For ascii, Tidy will accept Latin-1 (ISO-8859-1) character values, but will use entities for all characters whose value &gt; 127. For raw, Tidy will output values above 127 without translating them into entities. For latin1, characters above 255 will be written as entities. For utf8, Tidy assumes that both input and output is encoded as UTF-8. You can use iso2022 for files encoded using the ISO-2022 family of encodings e.g. ISO-2022-JP. For mac and win1252, Tidy will accept vendor specific character values, but will use entities for all characters whose value &gt; 127. For unsupported encodings, use an external utility to convert to and from UTF-8. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="input-encoding">input-encoding</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Encoding</strong><br />
            Default: <strong>latin1</strong><br />Example: <strong>raw, ascii, latin0, latin1, utf8, iso2022, mac, win1252, ibm858, utf16le, utf16be, utf16, big5, shiftjis</strong></td>
        <td align="right" valign="top">
          <a href="#char-encoding">char-encoding</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies the character encoding Tidy uses for the input. See char-encoding for more info. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="language">language</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>String</strong><br />
            Default: <strong>-</strong><br />Example: <strong>-</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">Currently not used, but this option specifies the language Tidy uses (for instance "en"). </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="newline">newline</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>enum</strong><br />
            Default: <strong><em>Platform dependent</em></strong><br />Example: <strong>LF, CRLF, CR</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">The default is appropriate to the current platform: CRLF on PC-DOS, MS-Windows and OS/2, CR on Classic Mac OS, and LF everywhere else (Unix and Linux). </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="output-bom">output-bom</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>AutoBool</strong><br />
            Default: <strong>auto</strong><br />Example: <strong>auto, y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should write a Unicode Byte Order Mark character (BOM; also known as Zero Width No-Break Space; has value of U+FEFF) to the beginning of the output; only for UTF-8 and UTF-16 output encodings. If set to "auto", this option causes Tidy to write a BOM to the output only if a BOM was present at the beginning of the input. A BOM is always written for XML/XHTML output using UTF-16 output encodings. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="output-encoding">output-encoding</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Encoding</strong><br />
            Default: <strong>ascii</strong><br />Example: <strong>raw, ascii, latin0, latin1, utf8, iso2022, mac, win1252, ibm858, utf16le, utf16be, utf16, big5, shiftjis</strong></td>
        <td align="right" valign="top">
          <a href="#char-encoding">char-encoding</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies the character encoding Tidy uses for the output. See char-encoding for more info. May only be different from input-encoding for Latin encodings (ascii, latin0, latin1, mac, win1252, ibm858). </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr valign="bottom">
        <td valign="top" colspan="2" class="h2" id="MiscellaneousReference">Miscellaneous Options Reference
    </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="error-file">error-file</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>String</strong><br />
            Default: <strong>-</strong><br />Example: <strong>-</strong></td>
        <td align="right" valign="top">
          <a href="#output-file">output-file</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies the error file Tidy uses for errors and warnings. Normally errors and warnings are output to "stderr". </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="force-output">force-output</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should produce output even if errors are encountered. Use this option with care - if Tidy reports an error, this means Tidy was not able to, or is not sure how to, fix the error, so the resulting output may not reflect your intention. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="gnu-emacs">gnu-emacs</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should change the format for reporting errors and warnings to a format that is more easily parsed by GNU Emacs. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="gnu-emacs-file">gnu-emacs-file</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>String</strong><br />
            Default: <strong>-</strong><br />Example: <strong>-</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">Used internally. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="keep-time">keep-time</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should keep the original modification time of files that Tidy modifies in place. The default is no. Setting the option to yes allows you to tidy files without causing these files to be uploaded to a web server when using a tool such as SiteCopy. Note this feature is not supported on some platforms. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="output-file">output-file</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>String</strong><br />
            Default: <strong>-</strong><br />Example: <strong>-</strong></td>
        <td align="right" valign="top">
          <a href="#error-file">error-file</a>
        </td>
      </tr>
      <tr>
        <td colspan="2">This option specifies the output file Tidy uses for markup. Normally markup is written to "stdout". </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="quiet">quiet</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should output the summary of the numbers of errors and warnings, or the welcome or informational messages. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="slide-style">slide-style</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>String</strong><br />
            Default: <strong>-</strong><br />Example: <strong>-</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">Currently not used.  Tidy Classic only. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="tidy-mark">tidy-mark</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>yes</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should add a meta element to the document head to indicate that the document has been tidied. Tidy won't add a meta element if one is already present. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
      <tr>
        <td class="tabletitle" valign="top" id="write-back">write-back</td>
        <td class="tabletitlelink" valign="top" align="right">
          <a href="#top">Top</a>
        </td>
      </tr>
      <tr>
        <td valign="top">Type: <strong>Boolean</strong><br />
            Default: <strong>no</strong><br />Example: <strong>y/n, yes/no, t/f, true/false, 1/0</strong></td>
        <td align="right" valign="top" />
      </tr>
      <tr>
        <td colspan="2">This option specifies if Tidy should write back the tidied markup to the same file it read from. You are advised to keep copies of important files before tidying them, as on rare occasions the result may not be what you expect. </td>
      </tr>
      <tr>
        <td> </td>
      </tr>
    </table>
  </body>
</html></zorba-rest:payload>