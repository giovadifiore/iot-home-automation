(function($){
	
	var $menu = $('#main-menu'),
		$menuItems = $menu.find('a'),
		$pagesContainer	= $('#pagesContainer'),
		$pages	= $pagesContainer.find('.page');
	
	$menuItems.click(function(){
		
		var $clicked = $(this);
		
		if ($clicked.hasClass('active-menu'))
		{
			// do-nothing if the menu item is already active
			return;
		}
		
		// update the menu item class
		$menuItems.filter('.active-menu').removeClass('active-menu');
		$clicked.addClass('active-menu');
		
		// get the requested page
		var requestedPage = $clicked.attr('href'),
			$activePage	= $pages.filter('.active-page'),
			$nextActivePage = $pages.filter('[data-page="'+requestedPage+'"]');
		
		// perform the animation
		$activePage.fadeOut(200, function(){
			$activePage.removeClass('active-page');
			$nextActivePage.fadeIn(200, function(){
				$nextActivePage.addClass('active-page');
				
				// at the end of the transition, call the onPageChange handler
				if (typeof $.onPageChange === 'function')
				{
					$.onPageChange($activePage.attr('data-page'), requestedPage);
				}				
			});
		});
	});
	
}(jQuery));