BEGIN {
	print "; Automatically generated file with config"
	print "; ----------------------------------------"
	# initialize substitution vars
	if ( NAME != "" ) SUBST["NAME"] = NAME;
	if ( LNG != "" ) SUBST["LNG"] = LNG;
	if ( VER != "" ) SUBST["VER"] = VER;
	if ( SRC != "" ) SUBST["SRC"] = SRC;
	# etc
}

function replace( str, src, dst ) {
	target = str
	pos = index( target, src );
	while (pos != 0) {
		target = substr( target, 1, pos-1 )  dst  substr( target, pos + length(src) );
		pos = index( target, src );
	}
	return target
}

function replace_all( str, subs ) {
	for( rep in subs ) {
		str = replace( str, sprintf("#%s#",rep), subs[ rep ] );
	}
	return str;
}

{
	# handle includes with appending
     if (NF > 2 && $1 == "@include") {
     	  filename = $2;
     	  if ( INC != "" ) filename = INC "\\" filename;
          while ((getline line < filename) > 0)
             print line $3
          close(filename)
     } else {
     	# handle substitutions
     	$0 = replace_all( $0, SUBST );
		# simply print out string
        print
	}
}