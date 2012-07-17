<?php if(!defined('OCTOMS')){header('HTTP/1.1 403');die('{"error":"forbidden"}');}
/*
 * @package       OctoMS
 * @link          http://octoms.com
 * @copyright     Copyright 2011, Valentino-Jivko Radosavlevici (http://valentino.radosavlevici.com)
 * @license       GPL v3.0 (http://www.gnu.org/licenses/gpl-3.0.txt)
 * 
 * Redistributions of files must retain the above copyright notice.
 * 
 * @since         OctoMS 0.0.1
 */
	
	/*
	 * Math library
	 * 
	 * @link http://octoms.com/doc/math_cl
	 * 
	 * @package OctoMS
	 * @subpackage octoms
	 * @version 0.1
	 * 
	 * @author Valentino-Jivko Radosavlevici
	 */
	class math_cl
	{		
		/**
		 * PI
		 * 
		 * @example 
		 * // Get the first 100 decimals of pi
		 * $math->pi(100);
		 * 
		 * @link http://octoms.com/doc/math_cl/pi
		 * 
		 * @param int $precision
		 * @return string - PI
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function pi($precision=20)
		{
			$precision = abs(intval($precision));
			
			if ($precision > 14)
			{
				// 1000 is enough
				$precision = $precision>1000?1000:$precision;
				
				// Why waste resources calculating the number some people know by heart? (aprox.)
				return substr('3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989', 
						0, 
						$precision + 2
				);
			}
			else 
			{
				return (string)pi();
			}
			
		}// end function pi()
		
	}// end class math_cl
	
	
/* End Of File <math.inc> */