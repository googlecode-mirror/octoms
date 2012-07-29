<?php if(!defined('OCTOMS')){header('HTTP/1.1 403');die('{"error":"forbidden"}');}
/**
 * @package       OctoMS
 * @link          http://octoms.com
 * @copyright     Copyright 2011, Valentino-Jivko Radosavlevici (http://valentino.radosavlevici.com)
 * @license       GPL v3.0 (http://www.gnu.org/licenses/gpl-3.0.txt)
 * 
 * Redistributions of files must retain the above copyright notice.
 * 
 * @since         OctoMS 0.0.1
 */	
	
	/**
	 * Wizard main controller
	 * 
	 * @link http://octoms.com/doc/Wizard
	 * 
	 * @package OctoMS
	 * @subpackage wizard
	 * @version 0.1
	 * 
	 * @author Valentino-Jivko Radosavlevici
	 */
	class wizard extends octoms
	{
		/**
		 * Constructor
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function __construct()
		{
			// Load the input library
			$this->_('input');
			
			// Load the HTML helper
			$this->_('html',OMS_CH);
			
			// Load the model
			$this->_('wizard',OMS_M,'w');
			
		}// end function __construct()
		
		/**
		 * Debugging
		 *
		 * @author Valentino-Jivko Radosavlevici
		 */
		function debug()
		{
			// The debug page parameter
			if(FALSE === ($debug=$this->input->post('debug'))) $this->w->append('No page specified.')->render();
			
			// Decide what to debug
			$term = (strpos($debug, '-')!==FALSE)?substr($debug, 0,strpos($debug, '-')):$debug;
			
			// Decide what to look for
			preg_match_all('%-([a-zA-Z=]+)+%', $debug, $m);
			$location = isset($m[1])?$m[1]:array();
			
			// Need help?
			if (in_array('help', $location))
			{
				$r = $this->w->help($term,'debug');
				$this->w->append(
					$this->view->load('partials/help',array('info'=>$r)),
					array('t')
				)->render();
			}
			
			// Initiate the debug request
			$r = $this->w->debug($debug);
			
			// Show the result
			if (is_object($r)) 
			{
				$this->w->append(
					$this->view->load('partials/debug',array('info'=>$r)),
					array('h','t','a')
				)->render();
			}
			elseif (is_string($r))
			{
				$this->w->append($r)->render();
			}
			else 
			{
				$this->w->append('The page you specified encountered a fatal error or does not exist on this server.')->render();
			}
		}
		
		/**
		 * Search
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function search()
		{
			// The search parameter
			if(FALSE === ($search=$this->input->post('search')))$this->w->append('No search data provided')->render();
			
			// Decide what to search for
			$term = (strpos($search, '-')!==FALSE)?substr($search, 0,strpos($search, '-')):$search;
			
			// Decide what to look for
			preg_match_all('%-([a-zA-Z=]+)+%', $search, $m);
			$location = isset($m[1])?$m[1]:array();
			
			// Need help?
			if (in_array('help', $location))
			{
				$r = $this->w->help($term,'search');
				$this->w->append(
					$this->view->load('partials/help',array('info'=>$r)),
					array('t')
				)->render();
			}
			
			// The todo list?
			if (in_array('todo', $location))
			{
				$this->task('todo');
			}
			
			// Just a local search then
			$r = $this->w->search($term,$location);
			$this->w->append(
				$this->view->load('partials/search',array('info'=>$r)),
				array('h','t')
			)->render();
			
		}// end function search()
		
		/**
		 * Tasks
		 * 
		 * @example 
		 * @link http://octoms.com/doc/wizard/task
		 * 
		 * @method task()
		 * @param 
		 * @return 
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function task($t=NULL)
		{
			if (empty($t))
			{
				$this->w->append("No task defined.")->render();
			}
			
			// Verify the task is allowed
			if (in_array($t, array('todo')))
			{
				// Plaintext
				header('content-type:application/json');
			
				// Get the raw server response
				$a=func_get_args();
				print $this->w->api(array('eng'=>implode('_',$a),'arg'=>$this->input->json('data')),'task');
			}
			else
			{
				$this->w->append("Invalid task.")->render();
			}
			
			// Nothing beyond this point
			exit();
			
		}// end function task()
		
	}// end class wizard
	
/* End Of File <wizard.inc> */