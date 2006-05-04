\ tag: PCI helper functions
\ 
\ Copyright (C) 2003-2004 Stefan Reinauer
\ Copyright (C) 2003 Samuel Rydh
\ 
\ See the file "COPYING" for further information about
\ the copyright and warranty status of this work.
\ 

\ simple set of words for pci access, these are not 
\ compliant to the PCI bus binding of OpenFirmware.

\ only forth
\ vocabulary pci 
\ also pci definitions

hex

: busdevfn ( bus dev fn -- busdevfn )
  7 and swap 
  1f and 3 << or       ( dev fn -- devfn )
  swap 8 << or            ( bus devfn -- busdevfn )
  ;

: config-command ( busdevfn reg -- reg addr )
  dup -rot
  3 invert and 
  swap 8 << or 
  80000000 or
  ;

: pci-c@ ( busdevfn reg -- x )
  config-command
  cf8 iol!
  3 and cfc + 
  ioc@
  ;

: pci-w@ ( busdevfn reg -- x )
  config-command
  cf8 iol!
  2 and cfc + iow@
  ;

: pci-l@ ( busdevfn reg -- x )
  config-command
  cf8 iol! 
  drop
  cfc iol@
  ;

: pci-c! ( busdevfn reg val -- )
  -rot config-command 
  cf8 iol! 
  3 and cfc + ioc!
  ;
  
: pci-w! ( busdevfn reg val -- )
  -rot config-command 
  cf8 iol! 
  2 and cfc + iow!
  ;
  
: pci-l! ( busdevfn reg val -- )
  -rot config-command 
  cf8 iol! 
  drop
  cfc iol!
  ;
 
: dump-pci-device ( bus dev fn -- )
  2 pick (.) type 3a emit over 
  (.) type 2e emit dup (.) type 20 emit 5b emit  \ 0:18.0 [
  busdevfn >r
  r@ 0 pci-w@ u. 2f emit r@ 2 pci-w@ u. 5d emit 	 \ 1022/1100]
  r>
  \ now we iterate
  10 0 do
    cr i todigit emit 30 emit 3a emit 20 emit
    10 0 do
      dup i j 4 << or  pci-c@ 
      dup 4 >> todigit emit f and todigit emit
      20 emit
    loop
  loop
  drop
  cr cr
  ;
  
\ : test-pci
\   0  2 0 dump-pci-device
\   ;

\ only forth


\ -------------------------------------------------------------------------
\ PCI encode/decode unit
\ -------------------------------------------------------------------------

\ we only implement DD and DD,F
: encode-unit-pci ( phys.lo phy.mid phys.hi -- str len )
  nip nip ff00 and 8 >> dup 3 >>
  swap 7 and
  ( ddddd fff )

  ?dup if
    pocket tohexstr
    " ," pocket tmpstrcat
  else
    0 0 pocket tmpstrcpy
  then
  >r
  rot pocket tohexstr r> tmpstrcat drop
;

: decode-unit-pci-bus ( str len bus -- phys.lo phys.mid phys.hi )
  -rot ascii , left-split
  ( addr-R len-R addr-L len-L )
  parse-hex b << f800 and
  -rot parse-hex 8 << 700 and
  or
  ( bus phys.hi )
  swap ff and 10 << or
  0 0 rot
;
 